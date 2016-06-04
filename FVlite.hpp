// FVlite.hpp
//
// Defines a class for finite volume calculations.
// Used to solve FVlite scattering problems

#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <string>
#include <iostream>
#include <fstream>

#include <libconfig.h++>

#include "Grid/Grid.hpp"
#include "Initialisers/Initialisers.hpp"
#include "Updaters/Updater.hpp"
#include "Sources/Sources.hpp"
#include "Output/Output.hpp"

using std::string;
using libconfig::Config;
using libconfig::Setting;

namespace FVlite{

class FT_Controller;

class Solver{
friend class FT_Controller;

private:

    Grid*        pGrid;
    Timer*       pTimer;
    Initialiser* pInit;
    Updater*     pUpdate;
    Output*      pOutput;

public:

    Solver(){}
    Solver( Config& cfg){ init(cfg);}
    ~Solver();

    void init( Config& cfg);

    bool is_complete();
    void advance();
    void solve();
    void exec();

    // PRINT FUNCTIONS, SHOULD BE DEPRECATED
    void printData(std::ofstream& file);
    void printGeometry();
    void printLevelSetVertices();
};

Solver::~Solver(){
  delete pUpdate;
  delete pInit;
  delete pTimer;
  delete pGrid;
}

void Solver::init( Config& cfg){ 

    // Set up grid
    std::cout << "Building grid.." << std::endl;
    pGrid = new Grid;
    Setting& gridCfg = cfg.lookup("Grid");
    pGrid->init(gridCfg);

    // Output
    std::cout << "Setting up output..." << std::endl;
    pOutput = new Output;
    Setting& outputCfg = cfg.lookup("Output");
    pOutput->init(pGrid,outputCfg);

    // Set up timer
    std::cout << "Building timer..." << std::endl;
    pTimer = new Timer;
    Setting& timerCfg = cfg.lookup("Timing");
    pTimer->init(pGrid,timerCfg);

    // Set up Initialiser
    string InitString  = cfg.lookup("Init.type");
    std::cout << "Building initialiser..." << std::endl;
    pInit = InitialiserFactory.create(InitString);
    pInit->init(pGrid);
    

    // Set up source
    std::cout << "Building PDE solver..." << std::endl;

    Source* pSource;
    bool TE    = true;
    SOURCE_TYPE STYPE = NOSOURCE;

    switch(STYPE){
        case NOSOURCE:
            pSource = new SourceNone();
            (void)TE;
            break;
#ifdef MAXWELL
        case SINE:
            pSource = new SourceSin( omega, TE);
            break;
        case COSINE:
            pSource = new SourceCos( omega, TE);
            break;
        case GAUSSIAN:
            pSource = new SourceGaussian( sigma, mean, TE);
            break;
        case GAUSSDER:
            pSource = new SourceGaussder( sigma, mean, TE);
            break;
#endif
        default:
            pSource = new SourceNone();
            (void)TE;
            break;
    }

    // Set up flux solver
    string FluxString  = cfg.lookup("FVM.scheme");
    string LimitString = cfg.lookup("FVM.limiter");
    FluxSolver* pFlux = FluxSolverFactory.create(FluxString);
    pFlux->init(pGrid,pSource,LimitString);

    // Set up finite volume solver
    string FvmString   = cfg.lookup("FVM.type");
    FVMsolver* pFVM = FVMsolverFactory.create(FvmString);
    pFVM->init(pGrid,pFlux,pSource);

    // Set up boundary update method
    std::cout << "Building boundary updater..." << std::endl;
    string BuString    = cfg.lookup("Boundaries.type");
    BoundaryUpdater* pBUpdate = BoundaryUpdaterFactory.create(BuString);
    pBUpdate->init(pGrid);

    // Set up updater
    std::cout << "Building complete time marching system..." << std::endl;
    pUpdate = new Updater(pGrid,pTimer,pFVM,pBUpdate);

    // Initialise
    std::cout << "Initialising..." << std::endl;
    pInit->exec();

    // Initial output
    pOutput->prod();

    std::cout << "Solver built successfully (probably)" << std::endl;
    return;
}

bool Solver::is_complete(){
    return pTimer->is_complete();
}

void Solver::advance(){
    pUpdate->exec();
    pOutput->prod();
    return;
}

void Solver::solve(){
    while( !pTimer->is_complete()){
        advance();
    }
    return;
}

void Solver::exec(){
    solve();
    pOutput->print();
    return;
}


// ADDITIONAL PRINT FUNCTIONS
// Should be deprecated!


void Solver::printData(std::ofstream& file){
#ifdef MAXWELL
    file << "# x y Ex Ey Ez Hx Hy Hz LevelSet" << std::endl;
#endif
#ifdef EULER
    file << "# x y rho rhoUx rhoUy E levelset" << std::endl;
#endif
    StateVector State;
    for( int ii=pGrid->startX(); ii<pGrid->endX(); ii++){
        for( int jj=pGrid->startY(); jj<pGrid->endY(); jj++){
            State = pGrid->state(ii,jj);
            file << pGrid->x(ii)  << '\t' << pGrid->y(jj)  << '\t';
            for( unsigned int kk=0; kk<State.size(); kk++){
                file << State[kk] << '\t';
            }
            file << (*pGrid->levelset())(ii,jj) << std::endl;
        }
        file << std::endl;
    }
    return;
}

void Solver::printGeometry(){
    std::ofstream file("geometry.dat");
    file << "# x y levelset betaL betaR betaT betaB alpha Xb[0] Xb[1] Xb[2] Nb[0] Nb[1] Nb[2]" << std::endl;
    int startX = pGrid->startX();
    int startY = pGrid->startY();
    int endX = pGrid->endX();
    int endY = pGrid->endY();
    BoundaryGeometry geometry;
    for( int ii=startX; ii<endX; ii++){
        for( int jj=startY; jj<endY; jj++){
            geometry = pGrid->boundary(ii,jj);
            file << pGrid->x(ii)  << '\t' << pGrid->y(jj)  << '\t';
            file << (*pGrid->levelset())(ii,jj) << '\t'; 
            file << geometry.betaL() << '\t';
            file << geometry.betaR() << '\t';
            file << geometry.betaT() << '\t';
            file << geometry.betaB() << '\t';
            file << geometry.alpha() << '\t';
            file << (geometry.Xb())[0] << '\t';
            file << (geometry.Xb())[1] << '\t';
            file << (geometry.Xb())[2] << '\t';
            file << (geometry.Nb())[0] << '\t';
            file << (geometry.Nb())[1] << '\t';
            file << (geometry.Nb())[2] << '\t';
            file << std::endl;
        }
        file << std::endl;
    }
    return;
}

void Solver::printLevelSetVertices(){
    std::ofstream file("vertices.dat");
    file << "# x y levelset" << std::endl;
    int sizeX = pGrid->sizeX();
    int sizeY = pGrid->sizeY();
    double x, y;
    double dx = pGrid->dx();
    double dy = pGrid->dy();
    for( int ii=1; ii<sizeX; ii++){
        x = pGrid->x(ii) - 0.5*dx;
        for( int jj=1; jj<sizeY; jj++){
            y = pGrid->y(jj) - 0.5*dy;
            file << x  << '\t' << y  << '\t';
            file << pGrid->levelset()->interpolate(x,y) << std::endl; 
        }
        file << std::endl;
    }
    return;
}

}// Namespace closure
#endif /* SOLVER_HPP */
