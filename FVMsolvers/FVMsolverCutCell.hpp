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

    std::cout << "starting sweep, " << dim << std::endl;
    switch(dim){
        case 'x' :
            ds = pGrid->dx();
            // Solve flux
            pFlux->exec(dim,t,dt);
            std::cout << "standard x fluces" << std::endl;
            pCutCell->correctFluxes(dim,dt);
            std::cout << "corrected x fluxes" << std::endl;
            // Explicit update formula -- Euler method
            for( int jj=startY; jj<endY; jj++){
                for( int ii=startX; ii<endX; ii++){
                    Boundary = pGrid->boundary(ii,jj);
                    alpha = Boundary.alpha();
                    betaL = Boundary.betaL();
                    betaR = Boundary.betaR();
                    if( alpha == 0.) continue;
                    if( alpha == 1.){
//                        pGrid->state(ii,jj) = pGrid->state(ii,jj) + (pGrid->flux(ii-1,jj)-pGrid->flux(ii,jj)) * dt/ds;
                    }else{
                        BoundaryFlux.set( pGrid->state_ref(ii,jj),dim);
                        FluxL = pGrid->flux(ii-1,jj);
                        FluxR = pGrid->flux(ii,jj);
//                        pGrid->state(ii,jj) += (betaL*FluxL - betaR*FluxR - (betaL-betaR)*BoundaryFlux) * dt/(ds*alpha);
                    }
                }
            }
            break;
        case 'y' :
            ds = pGrid->dy();
            // Solve flux
            pFlux->exec(dim,t,dt);
            std::cout << "standard y fluxes" << std::endl;
            pCutCell->correctFluxes(dim,dt);
            std::cout << "corrected y fluxes" << std::endl;
            // Explicit update formula -- Euler method
            for( int jj=startY; jj<endY; jj++){
                for( int ii=startX; ii<endX; ii++){
                    Boundary = pGrid->boundary(ii,jj);
                    alpha = Boundary.alpha();
                    betaL = Boundary.betaB();
                    betaR = Boundary.betaT();
                    if( alpha == 0.) continue;
                    if( alpha == 1.){
//                        pGrid->state(ii,jj) = pGrid->state(ii,jj) + (pGrid->flux(ii,jj-1)-pGrid->flux(ii,jj)) * dt/ds;
                    }else{
                       BoundaryFlux.set( pGrid->state_ref(ii,jj),dim);
                        FluxL = pGrid->flux(ii,jj-1);
                        FluxR = pGrid->flux(ii,jj);
//                        pGrid->state(ii,jj) += (betaL*FluxL - betaR*FluxR - (betaL-betaR)*BoundaryFlux) * dt/(ds*alpha);
                    }
                }
            }
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
