// FluxSolverHLLCfirstorder.hpp
//
// Finite volume flux solver.
// Applies HLLC approximate Riemann solver.
// Does not include linear reconstruction.

#ifndef FLUXHLLCFIRST_HPP
#define FLUXHLLCFIRST_HPP

#ifdef EULER

#include"FluxSolverAbstract.hpp"

using std::string;

namespace FVlite{

class FluxSolverHLLCfirst : public FluxSolver{
public:
    FluxSolverHLLCfirst(){}
    virtual void solve( char dim, double dt);
};

REGISTER(FluxSolver,HLLCfirst)

void FluxSolverHLLCfirst::solve( char dim, double dt){
    (void) dt;
    int startX = pGrid->startX();
    int startY = pGrid->startY();
    int endX   = pGrid->endX();
    int endY   = pGrid->endY();
    StateVector StateL, StateR;
    switch(dim){
        case 'x' :
            for( int jj=startY; jj<endY; jj++){
                for( int ii=startX-1; ii<endX; ii++){
                    StateL = pGrid->state(ii,jj);
                    StateR = pGrid->state(ii+1,jj);
                    pGrid->flux(ii,jj) = FluxSolver::HLLC_flux(dim,StateL,StateR);
                }
            }
            break;
        case 'y' :
            for( int jj=startY-1; jj<endY; jj++){
                for( int ii=startX; ii<endX; ii++){
                    StateL = pGrid->state(ii,jj);
                    StateR = pGrid->state(ii,jj+1);
                    pGrid->flux(ii,jj) = FluxSolver::HLLC_flux(dim,StateL,StateR);
                }
            }
            break;
        case 'z' :
            // Not yet implemented
            break;
    }
    return;
}

}// Namespace closure

#endif /* EULER */
#endif /* FLUXHLLCFIRST_HPP */
