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

namespace FVlite{

typedef std::pair<StateVector,StateVector> StatePair;

class FluxSolverMUSCLbase : public virtual FluxSolver{
public:
    virtual void solve( char dim, double dt);
    StatePair linear_reconstruction( double ds, double dt, char dim, StateVector& StateL, StateVector& StateCL, StateVector& CR, StateVector& StateR);
    StateVector getLimitedSlope(StateVector& StateL, StateVector& StateC, StateVector& StateR, double tol=1e-10, double omega=0.);
};

void FluxSolverMUSCLbase::solve( char dim, double dt){
    double ds;
    int startX = pGrid->startX();
    int startY = pGrid->startY();
    int endX = pGrid->endX();
    int endY = pGrid->endY();
    StateVector StateL, StateCL, StateCR, StateR;
    StatePair BarStates;
    FluxVector Flux;
    switch(dim){
        case 'x' :
            ds = pGrid->dx();
            for( int jj=startY; jj<endY; jj++){
                for( int ii=startX-1; ii<endX; ii++){
                    StateL  = pGrid->state(ii-1,jj);
                    StateCL = pGrid->state(ii,jj);
                    StateCR = pGrid->state(ii+1,jj);
                    StateR  = pGrid->state(ii+2,jj);
                    BarStates = linear_reconstruction(ds,dt,dim,StateL,StateCL,StateCR,StateR);
                    Flux = getIntercellFlux( ds, dt, dim, BarStates.first, BarStates.second);
                    pGrid->flux(ii,jj) = Flux;
                }
            }
            break;
        case 'y' :
            ds = pGrid->dy();
            for( int ii=startX; ii<endX; ii++){
                for( int jj=startY-1; jj<endY; jj++){
                    StateL  = pGrid->state(ii,jj-1);
                    StateCL = pGrid->state(ii,jj);
                    StateCR = pGrid->state(ii,jj+1);
                    StateR  = pGrid->state(ii,jj+2);
                    BarStates = linear_reconstruction(ds,dt,dim,StateL,StateCL,StateCR,StateR);
                    Flux = getIntercellFlux( ds, dt, dim, BarStates.first, BarStates.second);
                    pGrid->flux(ii,jj) = Flux;
                }
            }
            break;
        case 'z' :
            break;
    }
    return;
}


StatePair FluxSolverMUSCLbase::linear_reconstruction( double ds, double dt, char dim, StateVector& StateL, StateVector& StateCL, StateVector& StateCR, StateVector& StateR){
    StateVector DeltaL, DeltaR;
    StateVector InterL, InterR;
    FluxVector FluxL, FluxR;
    StateVector StateRbar, StateLbar;
    StatePair BarStates;

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
