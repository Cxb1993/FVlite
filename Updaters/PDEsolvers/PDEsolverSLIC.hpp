// PDEsolverSLIC.hpp
//
// Finite volume PDE solver
// Applies SLIC method.

#ifndef PDESLIC_HPP
#define PDESLIC_HPP

#include<cmath>

#include<iostream>

#include"PDEsolverAbstract.hpp"

namespace FVTD{

class PDEsolverSLIC : public PDEsolver{
public:
    PDEsolverSLIC( Grid* pGrid, LIMIT_TYPE LTYPE) : PDEsolver(pGrid,LTYPE) {}
    virtual void updateFlux( char dim, double dt);
    StateVector getLimiter(StateVector& L, StateVector& R, double omega);
};

void PDEsolverSLIC::updateFlux( char dim, double dt){

    double ds;
    double omega = 0.; // Set to default value
    double tol = 1e-10;
    int startX = pGrid->startX();
    int startY = pGrid->startY();
    int endX = pGrid->endX();
    int endY = pGrid->endY();

    StateVector DeltaL, DeltaR;
    StateVector Delta;
    StateVector Limiter;

    StateVector StateL, StateR;
    FluxVector FluxL, FluxR;
    StateVector StateLbar, StateRbar;
    FluxVector FluxFORCE;
    Material MatL, MatR;

    switch(dim){

        case 'x' :
            ds = pGrid->dx();
            for( int jj=startY; jj<endY; jj++){
                for( int ii=startX-1; ii<endX+1; ii++){
                    // Step 0: Get limited slope
                    DeltaL = pGrid->state(ii,jj) - pGrid->state(ii-1,jj);
                    DeltaR = pGrid->state(ii+1,jj) - pGrid->state(ii,jj);
                    // Impose minimum jump conditions
                    for( unsigned int kk=0; kk<DeltaL.size(); kk++){
                        if(fabs(DeltaL[kk]) < tol) DeltaL[kk] = tol*copysign(1.0,DeltaL[kk]);
                        if(fabs(DeltaR[kk]) < tol) DeltaR[kk] = tol*copysign(1.0,DeltaR[kk]);
                    }
                    Delta = 0.5*((1+omega)*DeltaL + (1-omega)*DeltaR);
                    Limiter = getLimiter(DeltaL,DeltaR,omega);
                    Delta *= Limiter;
                    // Step 1: Data reconstruction
                    StateL = pGrid->state(ii,jj) - 0.5*Delta;
                    StateR = pGrid->state(ii,jj) + 0.5*Delta;
                    // Step 2: Evolution
                    // Get fluxes
                    MatL = pGrid->material(ii,jj);
                    FluxL.set(StateL,MatL,dim);
                    FluxR.set(StateR,MatL,dim);
                    // If first iteration, calculate StateRbar and return to beginning of loop
                    if (ii==startX-1){
                        StateRbar = StateR + 0.5*(dt/ds)*( FluxL - FluxR);
                        continue;
                    }
                    StateLbar = StateL + 0.5*(dt/ds)*( FluxL - FluxR);
                    // Step 3: FORCE
                    FluxFORCE = FORCE_flux( ds, dt, dim, StateRbar, StateLbar, MatR, MatL);
                    // Step 5: Set flux
                    pGrid->flux(ii-1,jj) = FluxFORCE;
                    // Step 6: Calculate StateRbar for next step. MatR becomes MatL.
                    MatR = MatL;
                    StateRbar = StateR + 0.5*(dt/ds)*( FluxL - FluxR);
                }
            }
            break;
        case 'y' :
            ds = pGrid->dy();
            for( int ii=startX; ii<endX; ii++){
                for( int jj=startY-1; jj<endY+1; jj++){
                    // Step 0: Get limited slope
                    DeltaL = pGrid->state(ii,jj) - pGrid->state(ii,jj-1);
                    DeltaR = pGrid->state(ii,jj+1) - pGrid->state(ii,jj);
                    // Impose minimum jump conditions
                    for( unsigned int kk=0; kk<DeltaL.size(); kk++){
                        if(fabs(DeltaL[kk]) < tol) DeltaL[kk] = tol*copysign(1.0,DeltaL[kk]);
                        if(fabs(DeltaR[kk]) < tol) DeltaR[kk] = tol*copysign(1.0,DeltaR[kk]);
                    }
                    Delta = 0.5*((1+omega)*DeltaL + (1-omega)*DeltaR);
                    Limiter = getLimiter(DeltaL,DeltaR,omega);
                    Delta *= Limiter;
                    // Step 1: Data reconstruction
                    StateL = pGrid->state(ii,jj) - 0.5*Delta;
                    StateR = pGrid->state(ii,jj) + 0.5*Delta;
                    // Step 2: Evolution
                    // Get fluxes
                    MatL = pGrid->material(ii,jj);
                    FluxL.set(StateL,MatL,dim);
                    FluxR.set(StateR,MatL,dim);
                    // If first iteration, calculate StateRbar and return to beginning of loop
                    if (jj==startY-1){
                        StateRbar = StateR + 0.5*(dt/ds)*( FluxL - FluxR);
                        continue;
                    }
                    StateLbar = StateL + 0.5*(dt/ds)*( FluxL - FluxR);
                    // Step 3: FORCE
                    FluxFORCE = FORCE_flux( ds, dt, dim, StateRbar, StateLbar, MatR, MatL);
                    // Step 5: Set flux
                    pGrid->flux(ii,jj-1) = FluxFORCE;
                    // Step 6: Calculate StateRbar for next step. MatR becomes MatL.
                    MatR = MatL;
                    StateRbar = StateR + 0.5*(dt/ds)*( FluxL - FluxR);
                }
            }
            break;
        case 'z' :

            break;
    }
    return;
}

StateVector PDEsolverSLIC::getLimiter(StateVector& L, StateVector& R, double omega){
    StateVector r;
    StateVector result;
    r = L/R;
    result = pLimiter->exec( r, omega);
    return result;
}

}// Namespace closure
#endif /* PDESLIC_HPP */
