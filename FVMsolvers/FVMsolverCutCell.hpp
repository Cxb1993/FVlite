// PDEsolverCutCell.hpp
//
// Finite volume PDE solver.
// Time marches using Euler method.
// Rather than using a 'staircase' approximation at boundaries, uses the cut cell method.

#ifndef FVMCUTCELL_HPP
#define FVMCUTCELL_HPP

#include <iostream>
#include <cstdlib>

#include "FVMsolverAbstract.hpp"

namespace FVlite{

class FVMsolverCutCell : public FVMsolver{
public:
    virtual void exec( char dim, double t, double dt);
};

REGISTER(FVMsolver,CutCell)

void FVMsolverCutCell::exec( char dim, double t, double dt){
    int startX = pGrid->startX();
    int startY = pGrid->startY();
    int endX = pGrid->endX();
    int endY = pGrid->endY();
    double ds;
    StateVector State;
    BoundaryGeometry Boundary;
    FluxVector FluxL,FluxR,BoundaryFlux;
    double alpha, betaL, betaR;

    // Method:
    // 1) Sweep grid.
    // 2) If cell is filled (alpha=0), skip.
    // 3) If cell is free (alpha=1), standard explicit update.
    // 4) If cell is mixed, apply cut-cell update

    switch(dim){
        case 'x' :
#ifdef DEBUG
            std::cout<< "Beginning X sweep" << std::endl;
#endif
            ds = pGrid->dx();
            // Solve flux
            pFlux->exec(dim,t,dt);
            pCutCell->correctFluxes(dim,dt);
            // Explicit update formula -- Euler method
            for( int jj=startY; jj<endY; jj++){
                for( int ii=startX; ii<endX; ii++){
                    Boundary = pGrid->boundary(ii,jj);
                    alpha = Boundary.alpha();
                    betaL = Boundary.betaL();
                    betaR = Boundary.betaR();
                    if( alpha == 0.) continue;
                    if( alpha == 1.){
                        pGrid->state(ii,jj) = pGrid->state(ii,jj) + (pGrid->flux(ii-1,jj)-pGrid->flux(ii,jj)) * dt/ds;
                    }else{
                        BoundaryFlux.set( pGrid->state_ref(ii,jj),dim);
                        FluxL = pGrid->flux(ii-1,jj);
                        FluxR = pGrid->flux(ii,jj);
                        pGrid->state(ii,jj) += (betaL*FluxL - betaR*FluxR - (betaL-betaR)*BoundaryFlux) * dt/(ds*alpha);
                    }
                }
            }
#ifdef DEBUG
            std::cout<< "Finished X sweep" << std::endl;
#endif
            break;
        case 'y' :
#ifdef DEBUG
            std::cout<< "Beginning Y sweep" << std::endl;
#endif
            ds = pGrid->dy();
            // Solve flux
            pFlux->exec(dim,t,dt);
            pCutCell->correctFluxes(dim,dt);
            // Explicit update formula -- Euler method
            for( int jj=startY; jj<endY; jj++){
                for( int ii=startX; ii<endX; ii++){
                    Boundary = pGrid->boundary(ii,jj);
                    alpha = Boundary.alpha();
                    betaL = Boundary.betaB();
                    betaR = Boundary.betaT();
                    if( alpha == 0.) continue;
                    if( alpha == 1.){
                        pGrid->state(ii,jj) = pGrid->state(ii,jj) + (pGrid->flux(ii,jj-1)-pGrid->flux(ii,jj)) * dt/ds;
                    }else{
                       BoundaryFlux.set( pGrid->state_ref(ii,jj),dim);
                        FluxL = pGrid->flux(ii,jj-1);
                        FluxR = pGrid->flux(ii,jj);
                        pGrid->state(ii,jj) += (betaL*FluxL - betaR*FluxR - (betaL-betaR)*BoundaryFlux) * dt/(ds*alpha);
                    }
                }
            }
#ifdef DEBUG
            std::cout<< "Finished Y sweep" << std::endl;
#endif
            break;
        case 'z' :
            std::cerr << "Error, z direction not implemented" << std::endl;
            exit(EXIT_FAILURE);
            break;
        default:
            std::cerr << "Error, invalid direction specifier" << std::endl;
            exit(EXIT_FAILURE);
    }
    (void)betaL;
    (void)betaR;
    (void)ds;
    return;
}


}// Namespace closure
#endif /* FVMCUTCELL_HPP */
