// OperatorFluxSolver.hpp
//
// Operator which determines fluxes for finite volume scheme.
// Distinct from the FluxCalculator, as this writes fluxes to grid.
// It is also capable of performing more complicated tasks such as
// piece-wise linear reconstructions.

#ifndef OPERATORFLUXSOLVER_HPP
#define OPERATORFLUXSOLVER_HPP

#include <cstdlib>
#include <utility>
#include <iostream>
#include <string>

#include <libconfig.h++>

#include "Grid/Grid.hpp"
#include "FluxCalculators/FluxCalculators.hpp"
#include "Reconstructors/Reconstructors.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

typedef std::pair<StateVector,StateVector> StatePair;

class OperatorFluxSolver : public Operator {

protected:

    char m_dim;         // sweep direction for dimensional splitting
    double m_dt_ratio;  // the fraction of dt to advance each time step
    FluxCalculator* mpFluxCalculator;
    Reconstructor*  mpReconstructor;

public:

    OperatorFluxSolver(){}
    ~OperatorFluxSolver();

    void init( Setting& cfg);
    void exec( Grid& grid, Timer& timer);
    FluxVector getIntercellFlux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR);

};

// Function defintions

OperatorFluxSolver::~OperatorFluxSolver(){
    delete mpFluxCalculator;
    delete mpReconstructor;
}

void OperatorFluxSolver::init( Setting& cfg){
    // Get basic info
    m_dim = cfg.lookup("dim").c_str()[0];
    try{
        m_dt_ratio = cfg.lookup("dt_ratio");
    } catch ( const std::exception& e ){
        m_dt_ratio = 1.0;
    }
    // Get flux calculator and reconstruction scheme
    Setting& fluxSolverCfg = cfg.lookup("FluxSolver");
    string calcType, reconType, limitType;
    // Get flux calculator type
    try{
        calcType = fluxSolverCfg.lookup("type").c_str();
    } catch( const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    // Get reconstruction scheme. Rely on default if not specified.
    try{
        reconType = fluxSolverCfg.lookup("reconstructor").c_str();
    } catch( const std::exception&) {
        reconType = "Default";
    }

    mpFluxCalculator = FluxCalculatorFactory.create(calcType);
    mpReconstructor = ReconstructorFactory.create(reconType);
    mpReconstructor->init(cfg);
    return;
}

void OperatorFluxSolver::exec( Grid& grid, Timer& timer){
    double dt = timer.dt() * m_dt_ratio;
    double ds;
    int startX = grid.startX();
    int startY = grid.startY();
    int endX = grid.endX();
    int endY = grid.endY();
    StatePair States;
    FluxVector Flux;
    // Account for the fact that a row of N cells has N+1 interfaces which need
    // to be determined (including boundary ghost cells).
    switch(m_dim){
        case 'x' :
            ds = grid.dx();
            startX -= 1;
            break;
        case 'y' :
            ds = grid.dy();
            startY -= 1;
            break;
        case 'z' :
        default:
            ds = grid.dx();
    }
    // Sweep through grid, calculating each flux in turn
    for( int jj=startY; jj<endY; jj++){
        for( int ii=startX; ii<endX; ii++){
            States = mpReconstructor->exec( grid, ds, dt, m_dim, ii, jj);
            Flux = mpFluxCalculator->exec( ds, dt, m_dim, States.first, States.second);
            grid.flux(ii,jj) = Flux;
        }
    }
    return;
}

FluxVector OperatorFluxSolver::getIntercellFlux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR){
    return mpFluxCalculator->exec( ds, dt, dim, UL, UR);
}

}// Namespace closure
#endif /* OPERATORFLUXSOLVER_HPP */
