// FluxSolverStdBase.hpp
//
// Finite volume flux solver.
// Implements standard method to compute intercell fluxes.
// Does not use linear reconstruction.

#ifndef FLUXSTDBASE_HPP
#define FLUXSTDBASE_HPP
 
#include"FluxSolverAbstract.hpp"

namespace FVlite{

class FluxSolverStdBase : public virtual FluxSolver{
public:
    virtual void exec( char dim, double dt);
    virtual int stencilSize(){ return 2;}
};

void FluxSolverStdBase::exec( char dim, double dt){
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
                    pGrid->flux(ii,jj) = getIntercellFlux(ds,dt,dim,StateL,StateR);
                }
            }
            break;
        case 'y' :
            ds = pGrid->dy();
            for( int jj=startY-1; jj<endY; jj++){
                for( int ii=startX; ii<endX; ii++){
                    StateL = pGrid->state(ii,jj);
                    StateR = pGrid->state(ii,jj+1);
                    pGrid->flux(ii,jj) = getIntercellFlux(ds,dt,dim,StateL,StateR);
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
#endif /* FLUXSTDBASE_HPP */
