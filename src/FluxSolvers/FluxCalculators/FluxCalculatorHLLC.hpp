// FluxCalculatorHLLCbase.hpp
//
// Finite volume flux solver.
// Applies HLLC method.

#ifndef FLUXHLLC_HPP
#define FLUXHLLC_HPP

#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <ctgmath>

#include "FluxSolverAbstract.hpp"
#include "HLLCfunctions.hpp"

namespace FVlite{

class FluxCalculatorHLLC : public FluxCalculator{
public:
    virtual FluxVector getIntercellFlux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR);  
};

FluxVector FluxCalculatorHLLC::exec( double ds, double dt, char dim, const StateVector& StateL, const StateVector& StateR){
    // See 'Riemann Solvers & Numerical Methods for Fluid Dynamics',E Toro, section 10.6, for a summary of the HLLC fluxes
    (void)ds;
    (void)dt;

    // Step I: Calculate wave speeds
    Vector3 speeds;
    speeds = HLLC::getWaveSpeeds( dim, StateL, StateR);

    // Step II: Calculate HLLC state
    StateVector State_HLLC;
    State_HLLC = HLLC::getHLLCstate( dim, StateL, StateR, speeds);

    // Step III: Calculate flux
    // Determine which regime we're in, and calculate accordingly.
    FluxVector Flux_HLLC;
    Flux_HLLC = HLLC::getHLLCflux(dim,StateL,StateR,State_HLLC,speeds);

    return Flux_HLLC;
}


}// Namespace closure
#endif /* FLUXHLLC_HPP */
