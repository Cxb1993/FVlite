// FluxSolverFORCE.hpp
//
// Finite volume flux solver.
// Applies FORCE method.

#ifndef FLUXFORCE_HPP
#define FLUXFORCE_HPP

#include"FluxSolverAbstract.hpp"

namespace FVTD{

class FluxSolverFORCE : public FluxSolver{
public:
    FluxSolverFORCE( Grid* pGrid, Source* pSource, LIMIT_TYPE LTYPE=NONE) : FluxSolver(pGrid,pSource,LTYPE) {}
    virtual void solve(char dim, double dt);
};

void FluxSolverFORCE::solve( char dim, double dt){
    double ds;
    int startX = pGrid->startX();
    int startY = pGrid->startY();
    int endX   = pGrid->endX();
    int endY   = pGrid->endY();
    StateVector StateL, StateR;
    switch(dim){
        case 'x' :
            ds = pGrid->dx();
            for( int ii=startX-1; ii<endX; ii++){
                for( int jj=startY; jj<endY; jj++){
                    StateL = pGrid->state(ii,jj);
                    StateR = pGrid->state(ii+1,jj);
                    pGrid->flux(ii,jj) = FluxSolver::FORCE_flux(ds,dt,dim,StateL,StateR);
                }
            }
            break;
        case 'y' :
            ds = pGrid->dy();
            for( int ii=startX; ii<endX; ii++){
                for( int jj=startY-1; jj<endY; jj++){
                    StateL = pGrid->state(ii,jj);
                    StateR = pGrid->state(ii,jj+1);
                    pGrid->flux(ii,jj) = FluxSolver::FORCE_flux(ds,dt,dim,StateL,StateR);
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
#endif /* FLUXFORCE_HPP */
