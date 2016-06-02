// FVTD.hpp
//
// Defines a class for finite volume calculations.
// Used to solve FVTD scattering problems

#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <string>
#include <iostream>
#include <fstream>

#include "Vectors/Grid.hpp"
#include "Initialisers/Initialisers.hpp"
#include "Updaters/Updater.hpp"
#include "Sources/Sources.hpp"

using std::string;

namespace FVTD{

// TEMPORARY CONSTANTS
// REPLACE CODE, THIS IS BAD PRACTICE!

const double omega  = 1e10;  // Input frequency
const double radius = 0.125; // PEC circle radius
const double sigma = 1e-10;  // Gaussian source sigma
const double mean  = 4e-10;  // Gaussian source mean

class FT_Controller;

class Solver{
friend class FT_Controller;

private:

    std::ofstream datFile;
//    std::ofstream logFile;
 
    Grid*             pGrid;
    Timer*            pTimer;
    Initialiser*      pInit;
    Updater*          pUpdate;

public:

    Solver( std::string runName, int Nx, int Ny, double Lx, double Ly, double CFL, double tmax, string InitString, string FvmString, string FluxString, string BuString, string LimitString, SOURCE_TYPE STYPE=GAUSSDER);
    ~Solver();

    bool complete();
    void advance();
    void solve();
    void exec();

    void printData();
    void printData(std::ofstream& file);
    void printGeometry();
    void printLevelSetVertices();
    //void printLog();
};


Solver::Solver( std::string runName, int Nx, int Ny, double Lx, double Ly, double CFL, double tmax, string InitString, string FvmString, string FluxString, string BuString, string LimitString, SOURCE_TYPE STYPE){ 


    // Determine grid parameters
    std::cout << "Getting grid params..." << std::endl;
    int bound=2;
    std::cout << "Boundary size: " << bound << std::endl;

    // Set up grid
    std::cout << "Building grid.." << std::endl;
    pGrid   = new Grid(Nx,Ny,Lx,Ly,bound);

    // Set up timer
    std::cout << "Building timer..." << std::endl;
    pTimer  = new Timer(CFL,tmax,pGrid);

    // Set up Initialiser
    std::cout << "Building initialiser..." << std::endl;
    pInit = InitialiserFactory.create(InitString);
    pInit->init(pGrid);
    

    // Set up source
    std::cout << "Building PDE solver..." << std::endl;

    Source* pSource;
    bool TE    = true;

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
    FluxSolver* pFlux = FluxSolverFactory.create(FluxString);
    pFlux->init(pGrid,pSource,LimitString);

    // Set up finite volume solver
    FVMsolver* pFVM = FVMsolverFactory.create(FvmString);
    pFVM->init(pGrid,pFlux,pSource);

    // Set up boundary update method
    std::cout << "Building boundary updater..." << std::endl;
    BoundaryUpdater* pBUpdate = BoundaryUpdaterFactory.create(BuString);
    pBUpdate->init(pGrid);

    // Set up updater
    std::cout << "Building complete time marching system..." << std::endl;
    pUpdate = new Updater(pGrid,pTimer,pFVM,pBUpdate);

    // Set up output
    std::cout << "Setting up output..." << std::endl;
    std::string datSuffix(".dat");
    std::string datName = runName + datSuffix;
    datFile.open( datName.c_str());

    // Initialise
    std::cout << "Initialising..." << std::endl;
    pInit->exec();

    std::cout << "Solver built successfully (probably)" << std::endl;
}

Solver::~Solver(){
  delete pUpdate;
  delete pInit;
  delete pTimer;
  delete pGrid;
  datFile.close();
}

bool Solver::complete(){
    return pTimer->t() >= pTimer->tmax();
}

void Solver::advance(){
    pUpdate->exec();
    return;
}

void Solver::solve(){
    while( !pTimer->complete()){
        advance();
    }
    std::cout << std::endl;
    return;
}

void Solver::exec(){
    solve();
    printData();
    return;
}

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


void Solver::printData(){
    printData(datFile);
    return;
}

}// Namespace closure
#endif /* SOLVER_HPP */
