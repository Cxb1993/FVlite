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

#include "FluxCalculatorAbstract.hpp"
#include "HLLCfunctions.hpp"

namespace FVlite{

class FluxCalculatorHLLC : public FluxCalculator {
public:
    virtual FluxVector exec( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR, const Material& ML, const Material& MR);  
};

REGISTER(FluxCalculator,HLLC)

FluxVector FluxCalculatorHLLC::exec( double ds, double dt, char dim, const StateVector& StateL, const StateVector& StateR, const Material& ML, const Material& MR){
    // See 'Riemann Solvers & Numerical Methods for Fluid Dynamics',E Toro, section 10.6, for a summary of the HLLC fluxes
    (void)ds;
    (void)dt;
    (void)MR; // Assumes constant mat

    // Step I: Calculate wave speeds
    Vector3 speeds;
    speeds = HLLC::getWaveSpeeds( dim, StateL, StateR, ML);

    // Step II: Calculate HLLC state
    StateVector State_HLLC;
    State_HLLC = HLLC::getHLLCstate( dim, StateL, StateR, ML, speeds);

    // Step III: Calculate flux
    // Determine which regime we're in, and calculate accordingly.
    FluxVector Flux_HLLC;
    Flux_HLLC = HLLC::getHLLCflux(dim,StateL,StateR,ML,State_HLLC,speeds);

    return Flux_HLLC;
}


}// Namespace closure
#endif /* FLUXHLLC_HPP */
