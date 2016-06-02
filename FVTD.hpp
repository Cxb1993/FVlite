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

    Solver( std::string runName, int Nx, int Ny, double Lx, double Ly, double CFL, double tmax, INIT_TYPE ITYPE, FVM_TYPE FVMTYPE, FLUX_TYPE FTYPE, B_UPDATE_TYPE BTYPE, LIMIT_TYPE LTYPE= NONE, SOURCE_TYPE STYPE=GAUSSDER);
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


Solver::Solver( std::string runName, int Nx, int Ny, double Lx, double Ly, double CFL, double tmax, INIT_TYPE ITYPE, FVM_TYPE FVMTYPE, FLUX_TYPE FTYPE, B_UPDATE_TYPE BTYPE, LIMIT_TYPE LTYPE, SOURCE_TYPE STYPE){ 

    std::cout << "Getting grid params..." << std::endl;

    // Determine grid parameters
    int bound;
    switch(FTYPE){
        case FORCE :
            bound = 1;
        case RICHT:
            bound = 1;
            break;
        case SLIC:
            bound = 2;
            break;
#ifdef EULER
        case HLLC:
            bound = 2;
            break;
        case HLLCFIRST:
            bound = 1;
            break;
#endif
        default:
            bound = 1;
    }

    std::cout << "Boundary size: " << bound << std::endl;
    std::cout << "Building grid.." << std::endl;

    // Set up grid
    pGrid   = new Grid(Nx,Ny,Lx,Ly,bound);
 
    std::cout << "Building timer..." << std::endl;

    // Set up timer
    pTimer  = new Timer(CFL,tmax,pGrid);

    std::cout << "Building initialiser..." << std::endl;

    // Set up initialiser
    switch(ITYPE){
        case ZERO:
            pInit = new InitialiserZero(pGrid);
            break;
        case CIRCLE:
            pInit = new InitialiserCircle(pGrid);
            break;
        case RECTANGLE:
            pInit = new InitialiserRectangle(pGrid);
            break;
        case SLOPE:
            pInit = new InitialiserSlope(pGrid);
            break;
        case EXPLODE:
            pInit = new InitialiserExplosiveTest(pGrid);
            break;
        default:
            pInit = new InitialiserZero(pGrid);
    }
    
    std::cout << "Building PDE solver..." << std::endl;

    // Set up source

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
    FluxSolver* pFlux;

    switch(FTYPE){
        case FORCE:
            pFlux = new FluxSolverFORCE( pGrid, pSource);
            break;
        case RICHT:
            pFlux = new FluxSolverRichtmyer(pGrid, pSource);
            break;
        case SLIC:
            pFlux = new FluxSolverSLIC( pGrid, pSource, LTYPE);
            break;
#ifdef EULER
        case HLLC:
            pFlux = new FluxSolverHLLC( pGrid, pSource, LTYPE);
            break;
        case HLLCFIRST:
            pFlux = new FluxSolverHLLCfirst( pGrid, pSource, LTYPE);
            break;
#endif
        default:
            pFlux = new FluxSolverRichtmyer( pGrid, pSource);
            break;
    }

    // Set up finite volume solver
    FVMsolver* pFVM;
    switch(FVMTYPE){
        case FVMSTD:
            pFVM = new FVMsolverStd( pGrid, pFlux, pSource);
            break;
        default:
            pFVM = new FVMsolverStd( pGrid, pFlux, pSource);
            break;
    }

    std::cout << "Building boundary updater..." << std::endl;

    // Set up boundary update method
    BoundaryUpdater* pBUpdate;
    switch(BTYPE){
        case TRANSMISSIVE:
            pBUpdate = new BoundaryUpdaterTransmissive(pGrid);
            break;
        case CONSTANT:
            pBUpdate = new BoundaryUpdaterConstant(pGrid);
            break;
        default:
            pBUpdate = new BoundaryUpdaterTransmissive(pGrid);
    }

    std::cout << "Building complete time marching system..." << std::endl;

    // Set up updater
    pUpdate = new Updater(pGrid,pTimer,pFVM,pBUpdate);

    std::cout << "Setting up output..." << std::endl;

    // Set up output
    std::string datSuffix(".dat");
//    std::string logSuffix(".log");
    std::string datName = runName + datSuffix;
//    std::string logName = runName + logSuffix;
    datFile.open( datName.c_str());
//    logFile.open( logName.c_str());

    std::cout << "Initialising..." << std::endl;

    // Initialise
    pInit->exec();

    std::cout << "Solver built successfully (probably)" << std::endl;
}

Solver::~Solver(){
  delete pUpdate;
  delete pInit;
  delete pTimer;
  delete pGrid;
  datFile.close();
//  logFile.close();
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
//    printLog();
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
/*
void Solver::printLog(){
    logFile << "Code in progress, log files not ready!" << std::endl;
    return;
}
*/

}// Namespace closure
#endif /* SOLVER_HPP */
