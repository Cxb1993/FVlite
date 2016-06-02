// FluxSolverMUSCLbase.hpp
//
// Finite volume flux solver.
// Abstract MUSCL method -- computes the linear reconstruction, but does not solve the Riemann problem.
// Derived from the general flux solver class, used as a base for the likes of SLIC and MUSCL-Hancock.

#ifndef FLUXMUSCLBASE_HPP
#define FLUXMUSCLBASE_HPP
 
#include <cstdlib>
#include <cmath>
#include <utility>
#include <iostream>

#include"FluxSolverAbstract.hpp"

namespace FVTD{

typedef std::pair<StateVector,StateVector> StatePair;

class FluxSolverMUSCLbase : public FluxSolver{

public:

    FluxSolverMUSCLbase( Grid* pGrid, Source* pSource, LIMIT_TYPE LTYPE) : FluxSolver(pGrid,pSource,LTYPE) {}

    StatePair linear_reconstruction( StateVector& StateL, StateVector& StateCL, StateVector& CR, StateVector& StateR, char dim, double dt);
    StateVector getLimitedSlope(StateVector& StateL, StateVector& StateC, StateVector& StateR, double tol=1e-10, double omega=0.);

    virtual void solve( char dim, double dt) = 0;
};

StatePair FluxSolverMUSCLbase::linear_reconstruction( StateVector& StateL, StateVector& StateCL, StateVector& StateCR, StateVector& StateR, char dim, double dt){

    StateVector DeltaL, DeltaR;
    StateVector InterL, InterR;
    FluxVector FluxL, FluxR;
    StateVector StateRbar, StateLbar;
    StatePair BarStates;

    double ds;
    switch(dim){
        case 'x' :
            ds = pGrid->dx();
            break;
        case 'y' :
            ds = pGrid->dy();
            break;
        default :
            std::cerr << "ERROR: INCORRECT DIM, LINEAR RECONSTRUCTION" << std::endl;
            exit(EXIT_FAILURE);
    }

    DeltaL = getLimitedSlope( StateL, StateCL, StateCR);
    DeltaR = getLimitedSlope( StateCL, StateCR, StateR);

    // Reconstruction on left
    //
    // Step 1: Data reconstruction
    InterL = StateCL - 0.5*DeltaL;
    InterR = StateCL + 0.5*DeltaL;
    //
    // Step 2: Get fluxes
    FluxL.set(InterL,dim);
    FluxR.set(InterR,dim);
    //
    // Set 3: Evolve half timestep, get boundary state
    StateRbar = InterR + 0.5*(dt/ds)*( FluxL - FluxR);

    // Reconstruction on left
    //
    // Step 1: Data reconstruction
    InterL = StateCR - 0.5*DeltaR;
    InterR = StateCR + 0.5*DeltaR;
    //
    // Step 2: Get Fluxes
    FluxL.set(InterL,dim);
    FluxR.set(InterR,dim);
    // Set 3: Evolve half timestep, get boundary state
    StateLbar = InterL + 0.5*(dt/ds)*( FluxL - FluxR);

    BarStates = std::make_pair(StateRbar,StateLbar);
    return BarStates;
}

StateVector FluxSolverMUSCLbase::getLimitedSlope(StateVector& StateL, StateVector& StateC, StateVector& StateR, double tol, double omega){
    StateVector  DeltaL, DeltaR, r, Limiter, Delta;
    DeltaL = StateC - StateL;
    DeltaR = StateR - StateC;;
    // Impose minimum jump conditions
    for( unsigned int kk=0; kk<DeltaL.size(); kk++){
        if(fabs(DeltaL[kk]) < tol) DeltaL[kk] = tol*copysign(1.0,DeltaL[kk]);
        if(fabs(DeltaR[kk]) < tol) DeltaR[kk] = tol*copysign(1.0,DeltaR[kk]);
    }
    r = DeltaL/DeltaR; // component wise division
    Limiter = pLimiter->exec( r, omega);
    Delta = 0.5*Limiter*((1+omega)*DeltaL + (1-omega)*DeltaR); // Weight to one direction or the other. By default, no weighting, omega=0.
    return Delta;
}

}// Namespace closure
#endif /* FLUXMUSCLBASE_HPP */
