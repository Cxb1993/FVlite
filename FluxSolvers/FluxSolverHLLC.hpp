// FluxSolverHLLC.hpp
//
// Finite volume flux solver, derived from MUSLC base.
// Applies HLLC approximate Riemann solver

#ifndef FLUXHLLC_HPP
#define FLUXHLLC_HPP

#ifdef EULER

#include <cstdlib>
#include <cmath>
#include <iostream>

#include"FluxSolverMUSCLbase.hpp"

namespace FVTD{

class FluxSolverHLLC : public FluxSolverMUSCLbase{
public:
    FluxSolverHLLC( Grid* pGrid, Source* pSource, LIMIT_TYPE LTYPE) : FluxSolverMUSCLbase(pGrid,pSource,LTYPE) {}
    virtual void solve( char dim, double dt);
};

void FluxSolverHLLC::solve( char dim, double dt){
    int startX = pGrid->startX();
    int startY = pGrid->startY();
    int endX = pGrid->endX();
    int endY = pGrid->endY();
    StateVector StateL, StateCL, StateCR, StateR;
    StatePair BarStates;
    FluxVector Flux;
    switch(dim){
        case 'x' :
            for( int jj=startY; jj<endY; jj++){
                for( int ii=startX-1; ii<endX; ii++){
                    StateL  = pGrid->state(ii-1,jj);
                    StateCL = pGrid->state(ii,jj);
                    StateCR = pGrid->state(ii+1,jj);
                    StateR  = pGrid->state(ii+2,jj);
                    BarStates = linear_reconstruction(StateL,StateCL,StateCR,StateR,dim,dt);
                    Flux = FluxSolver::HLLC_flux( dim, BarStates.first, BarStates.second);
                    pGrid->flux(ii,jj) = Flux;
                }
            }
            break;
        case 'y' :
            for( int ii=startX; ii<endX; ii++){
                for( int jj=startY-1; jj<endY; jj++){
                    StateL  = pGrid->state(ii,jj-1);
                    StateCL = pGrid->state(ii,jj);
                    StateCR = pGrid->state(ii,jj+1);
                    StateR  = pGrid->state(ii,jj+2);
                    BarStates = linear_reconstruction(StateL,StateCL,StateCR,StateR,dim,dt);
                    Flux = FluxSolver::HLLC_flux( dim, BarStates.first, BarStates.second);
                    pGrid->flux(ii,jj) = Flux;
                }
            }
            break;
        case 'z' :
            break;
    }
    return;
}


}// Namespace closure
#endif /* EULER */
#endif /* FLUXHLLC_HPP */
