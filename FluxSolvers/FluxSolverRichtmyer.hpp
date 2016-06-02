// FluxSolverRichtmyer.hpp
//
// Finite volume flux solver.
// Applies Richtmyer's method.

#ifndef FLUXRICHT_HPP
#define FLUXRICHT_HPP

#include"FluxSolverAbstract.hpp"

namespace FVTD{

class FluxSolverRichtmyer : public FluxSolver{
public:
    FluxSolverRichtmyer( Grid* pGrid, Source* pSource, LIMIT_TYPE LTYPE=NONE) : FluxSolver(pGrid,pSource,LTYPE) {}
    virtual void solve( char dim, double dt);
};

void FluxSolverRichtmyer::solve( char dim, double dt){
    double ds;
    int startX = pGrid->startX();
    int startY = pGrid->startY();
    int endX   = pGrid->endX();
    int endY   = pGrid->endY();
    StateVector StateL, StateR;
    switch(dim){
        case 'x' :
            ds = pGrid->dx();
            for( int jj=startY; jj<endY; jj++){
                for( int ii=startX-1; ii<endX; ii++){
                    StateL = pGrid->state(ii,jj);
                    StateR = pGrid->state(ii+1,jj);
                    pGrid->flux(ii,jj) = FluxSolver::Richtmyer_flux(ds,dt,dim,StateL,StateR);
                }
            }
            break;
        case 'y' :
            ds = pGrid->dy();
            for( int jj=startY-1; jj<endY; jj++){
                for( int ii=startX; ii<endX; ii++){
                    StateL = pGrid->state(ii,jj);
                    StateR = pGrid->state(ii,jj+1);
                    pGrid->flux(ii,jj) = FluxSolver::Richtmyer_flux(ds,dt,dim,StateL,StateR);
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
#endif /* FLUXRICHT_HPP */
