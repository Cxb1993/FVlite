// ExplicitUpdaterEuler.hpp
//
// Finite volume explicit update calculator.
// Uses Euler method to march forward in time.
// Note that 'Euler' refers to the approximate method
// for solving ODEs, not the Euler equations!

#ifndef EXPLICITUPDATEREULER_HPP
#define EXPLICITUPDATEREULER_HPP

#include <cstdlib>
#include <utility>
#include <iostream>
#include <string>

#include <libconfig.h++>

#include "ExplicitUpdaters/ExplicitUpdater.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

typedef std::pair<StateVector,StateVector> StatePair;

class ExplicitUpdaterEuler : public ExplicitUpdater {
    void exec(char dim, double dt, double ds, int ii, int jj);
};

// Function defintions

FluxSolver::~FluxSolver(){
    delete mpFluxCalculator;
    delete mpReconstructor;
}

void FluxSolver::init( Grid* pGrid, Setting& cfg){
    string calcType, reconType, limitType;
    // Get flux calculator type
    try{
        calcType = cfg.lookup("type").c_str();
    } catch( const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    // Get reconstruction scheme. Rely on default if not specified.
    try{
        reconType = cfg.lookup("reconstructor").c_str();
    } catch( const std::exception&) {
        reconType = "Default";
    }

    mpGrid = pGrid;
    mpFluxCalculator = FluxCalculatorFactory.create(calcType);
    mpReconstructor = ReconstructorFactory.create(reconType);
    mpReconstructor->init(pGrid,cfg);
    return;
}

void FluxSolver::exec( char dim, double dt){
    double ds;
    int startX = mpGrid->startX();
    int startY = mpGrid->startY();
    int endX = mpGrid->endX();
    int endY = mpGrid->endY();
    StatePair States;
    FluxVector Flux;
    // Account for the fact that a row of N cells has N+1 interfaces which need
    // to be determined (including boundary ghost cells).
    switch(dim){
        case 'x' :
            ds = mpGrid->dx();
            startX -= 1;
            break;
        case 'y' :
            ds = mpGrid->dy();
            startY -= 1;
            break;
        case 'z' :
        default:
            ds = mpGrid->dx();
    }
    // Sweep through grid, calculating each flux in turn
    for( int jj=startY; jj<endY; jj++){
        for( int ii=startX; ii<endX; ii++){
            States = mpReconstructor->exec( ds, dt, dim, ii, jj);
            Flux = mpFluxCalculator->exec( ds, dt, dim, States.first, States.second);
            mpGrid->flux(ii,jj) = Flux;
        }
    }
    return;
}

FluxVector FluxSolver::getIntercellFlux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR){
    return mpFluxCalculator->exec( ds, dt, dim, UL, UR);
}

}// Namespace closure
#endif /* EXPLICITUPDATEREULER_HPP */
