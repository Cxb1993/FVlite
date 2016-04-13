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

namespace FVTD{

class Solver{

private:

    std::ofstream datFile;
//    std::ofstream logFile;
 
    Grid*             pGrid;
    Timer*            pTimer;
    Initialiser*      pInit;
    Updater*          pUpdate;

public:

    Solver( std::string runName, int Nx, int Ny, double Lx, double Ly, double CFL, double tmax, INIT_TYPE ITYPE, PDE_TYPE PTYPE, B_UPDATE_TYPE BTYPE, LIMIT_TYPE LTYPE= NONE);
    ~Solver();

    bool complete();
    void advance();
    void solve();
    void exec();
//    void exec_alt();

    void printData();
    void printData(std::ofstream& file);
    //void printLog();
};


Solver::Solver( std::string runName, int Nx, int Ny, double Lx, double Ly, double CFL, double tmax, INIT_TYPE ITYPE, PDE_TYPE PTYPE, B_UPDATE_TYPE BTYPE, LIMIT_TYPE LTYPE){ 

    std::cout << "Getting grid params..." << std::endl;

    // Determine grid parameters
    int bound;
    switch(PTYPE){
        case FORCE :
            bound = 1;
        case RICHT:
            bound = 1;
            break;
        case SLIC:
            bound = 2;
            break;
        default:
            bound = 1;
    }

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
        default:
            pInit = new InitialiserZero(pGrid);
    }
    
    std::cout << "Building PDE solver..." << std::endl;

    // Set up PDE solver
    // TEMPORARY LINE: LTYPE UNUSED
    (void)LTYPE;
    PDEsolver* pPDE;
    switch(PTYPE){
        case FORCE:
            pPDE = new PDEsolverFORCE( pGrid);
            break;
        case RICHT:
            pPDE = new PDEsolverRichtmyer(pGrid);
            break;
        case SLIC:
            pPDE = new PDEsolverSLIC( pGrid, LTYPE);
            break;
        default:
            pPDE = new PDEsolverRichtmyer( pGrid);
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
            break;
    }

    std::cout << "Building complete time marching system..." << std::endl;

    // Set up updater
    pUpdate = new Updater(pGrid,pTimer,pPDE,pBUpdate);

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
    return;
}

void Solver::exec(){
    solve();
    printData();
//    printLog();
    return;
}

void Solver::printData(std::ofstream& file){
    file << "# x\ty\tEx\tEy\tEz\tHx\tHy\tHz" << std::endl;
    StateVector State;
    int bound = pGrid->bound();
    double dx = pGrid->dx();
    double dy = pGrid->dy();
    for( int ii=pGrid->startX(); ii<pGrid->endX(); ii++){
        for( int jj=pGrid->startY(); jj<pGrid->endY(); jj++){
            State = pGrid->state(ii,jj);
            file << (ii-bound)*dx << '\t'
                 << (jj-bound)*dy << '\t'
                 << State.Ex()    << '\t'
                 << State.Ey()    << '\t'
                 << State.Ez()    << '\t'
                 << State.Hx()    << '\t'
                 << State.Hy()    << '\t'
                 << State.Hz()    << std::endl;
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
