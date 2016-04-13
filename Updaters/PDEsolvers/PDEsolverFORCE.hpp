// PDEsolverFORCE.hpp
//
// Finite volume PDE solver.
// Applies FORCE method.

#ifndef PDEFORCE_HPP
#define PDEFORCE_HPP

#include"PDEsolverAbstract.hpp"

namespace FVTD{

class PDEsolverFORCE : public PDEsolver{
public:
    PDEsolverFORCE( Grid* pGrid, LIMIT_TYPE LTYPE=NONE) : PDEsolver(pGrid,LTYPE) {}
    virtual void updateFlux(char dim, double dt);
};

void PDEsolverFORCE::updateFlux( char dim, double dt){
    double ds;
    int startX = pGrid->startX();
    int startY = pGrid->startY();
    int endX   = pGrid->endX();
    int endY   = pGrid->endY();
    StateVector StateL;
    StateVector StateR;
    Material MatL;
    Material MatR;
    switch(dim){
        case 'x' :
            ds = pGrid->dx();
            for( int ii=startX-1; ii<endX; ii++){
                for( int jj=startY; jj<endY; jj++){
                    StateL = pGrid->state(ii,jj);
                    StateR = pGrid->state(ii+1,jj);
                    MatL   = pGrid->material(ii,jj);
                    MatR   = pGrid->material(ii+1,jj);
                    pGrid->flux(ii,jj) = FORCE_flux(ds,dt,dim,StateL,StateR,MatL,MatR);
                }
            }
            break;
        case 'y' :
            ds = pGrid->dy();
            for( int ii=startX; ii<endX; ii++){
                for( int jj=startY-1; jj<endY; jj++){
                    StateL = pGrid->state(ii,jj);
                    StateR = pGrid->state(ii,jj+1);
                    MatL   = pGrid->material(ii,jj);
                    MatR   = pGrid->material(ii,jj+1);
                    pGrid->flux(ii,jj) = FORCE_flux(ds,dt,dim,StateL,StateR,MatL,MatR);
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
#endif /* PDEFORCE_HPP */
