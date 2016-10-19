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

#ifdef EULER
#define DEFAULTTOL 1e-10
#endif

#ifdef MAXWELL
#define DEFAULTTOL 1e-20
#endif

namespace FVlite{

typedef std::pair<StateVector,StateVector> StatePair;

class FluxSolverMUSCLbase : public virtual FluxSolver{
public:
    virtual void exec( char dim, double dt);
    virtual int stencilSize(){ return 4;}
    StatePair linear_reconstruction( double ds, double dt, char dim, StateVector& StateL, StateVector& StateCL, StateVector& CR, StateVector& StateR);
    StateVector getLimitedSlope(StateVector& StateL, StateVector& StateC, StateVector& StateR, double tol=DEFAULTTOL, double omega=0.);
};

void FluxSolverMUSCLbase::exec( char dim, double dt){
    double ds;
    int startX = pGrid->startX();
    int startY = pGrid->startY();
    int endX = pGrid->endX();
    int endY = pGrid->endY();
    BoundaryGeometry BoundaryL, BoundaryCL, BoundaryCR, BoundaryR;
    StateVector StateL, StateCL, StateCR, StateR;
    StatePair BarStates;
    FluxVector Flux;
    switch(dim){
        case 'x' :
            ds = pGrid->dx();
            for( int jj=startY; jj<endY; jj++){
                for( int ii=startX-1; ii<endX; ii++){
                    BoundaryL  = pGrid->boundary(ii-1,jj);
                    BoundaryCL = pGrid->boundary(ii,jj);
                    BoundaryCR = pGrid->boundary(ii+1,jj);
                    BoundaryR  = pGrid->boundary(ii+2,jj);
                    if( BoundaryL.alpha()==0. || BoundaryCL.alpha()==0. || BoundaryCR.alpha()==0. || BoundaryR.alpha()==0.){
                        StateL = pGrid->state(ii,jj);
                        StateR = pGrid->state(ii+1,jj);
                        Flux = getIntercellFlux( ds, dt, dim, StateL, StateR);
                    } else {
                        StateL  = pGrid->state(ii-1,jj);
                        StateCL = pGrid->state(ii,jj);
                        StateCR = pGrid->state(ii+1,jj);
                        StateR  = pGrid->state(ii+2,jj);
                        BarStates = linear_reconstruction(ds,dt,dim,StateL,StateCL,StateCR,StateR);
                        Flux = getIntercellFlux( ds, dt, dim, BarStates.first, BarStates.second);
                    }
                    pGrid->flux(ii,jj) = Flux;
                }
            }
            break;
        case 'y' :
            ds = pGrid->dy();
            for( int ii=startX; ii<endX; ii++){
                for( int jj=startY-1; jj<endY; jj++){
                    BoundaryL  = pGrid->boundary(ii,jj-1);
                    BoundaryCL = pGrid->boundary(ii,jj);
                    BoundaryCR = pGrid->boundary(ii,jj+1);
                    BoundaryR  = pGrid->boundary(ii,jj+2);
                    if( BoundaryL.alpha()==0. || BoundaryCL.alpha()==0. || BoundaryCR.alpha()==0. || BoundaryR.alpha()==0.){
                        StateL = pGrid->state(ii,jj);
                        StateR = pGrid->state(ii,jj+1);
                        Flux = getIntercellFlux( ds, dt, dim, StateL, StateR);
                    } else {
                        StateL  = pGrid->state(ii,jj-1);
                        StateCL = pGrid->state(ii,jj);
                        StateCR = pGrid->state(ii,jj+1);
                        StateR  = pGrid->state(ii,jj+2);
                        BarStates = linear_reconstruction(ds,dt,dim,StateL,StateCL,StateCR,StateR);
                        Flux = getIntercellFlux( ds, dt, dim, BarStates.first, BarStates.second);
                    }
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
