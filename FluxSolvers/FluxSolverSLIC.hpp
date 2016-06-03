// FluxSolverSLIC.hpp
//
// Finite volume flux solver, derived from MUSLC base.
// Applies SLIC method.

#ifndef FLUXSLIC_HPP
#define FLUXSLIC_HPP

#include<cmath>

#include<iostream>

#include"FluxSolverMUSCLbase.hpp"

using std::string;

namespace FVlite{

class FluxSolverSLIC : public FluxSolverMUSCLbase{
public:
    FluxSolverSLIC(){}
    virtual void solve( char dim, double dt);
};

REGISTER(FluxSolver,SLIC)

void FluxSolverSLIC::solve( char dim, double dt){
    double ds;
    int startX = pGrid->startX();
    int startY = pGrid->startY();
    int endX = pGrid->endX();
    int endY = pGrid->endY();
    StateVector StateL, StateCL, StateCR, StateR;
    StatePair BarStates;
    FluxVector FluxFORCE;
    switch(dim){
        case 'x' :
            ds = pGrid->dx();
            for( int jj=startY; jj<endY; jj++){
                for( int ii=startX-1; ii<endX; ii++){
                    StateL  = pGrid->state(ii-1,jj);
                    StateCL = pGrid->state(ii,jj);
                    StateCR = pGrid->state(ii+1,jj);
                    StateR  = pGrid->state(ii+2,jj);
                    BarStates = linear_reconstruction(StateL,StateCL,StateCR,StateR,dim,dt);
                    FluxFORCE = FluxSolver::FORCE_flux( ds, dt, dim, BarStates.first, BarStates.second);
                    pGrid->flux(ii,jj) = FluxFORCE;
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
                    BarStates = linear_reconstruction(StateL,StateCL,StateCR,StateR,dim,dt);
                    FluxFORCE = FluxSolver::FORCE_flux( ds, dt, dim, BarStates.first, BarStates.second);
                    pGrid->flux(ii,jj) = FluxFORCE;
                }
            }
            break;
        case 'z' :
            break;
    }
    return;
}

}// Namespace closure
#endif /* FLUXSLIC_HPP */
