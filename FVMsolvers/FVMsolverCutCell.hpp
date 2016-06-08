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

    FVMsolverCutCell(){}
    virtual void exec( char dim, double t, double dt);

    // Main cut cell function, used to calculate boundary, auxiliary and modified fluxes throughout grid
    void updateCutFluxes( char dim);

    // Component functions used in cut cell scheme
    double getAlphaShielded( const BoundaryGeometry& Boundary, char dim);
    FluxVector getBoundaryFlux( int ii, int jj, char dim);
    FluxVector getShieldedFlux( int ii, int jj, char dim);
    FluxVector getAuxFlux( int ii, int jj, char dim);
    FluxVector getModifiedFlux( int ii, int jj, char dim);

};

REGISTER(FVMsolver,CutCell)

void FVMsolverCutCell::exec( char dim, double t, double dt){
    int startX = pGrid->startX();
    int startY = pGrid->startY();
    int endX = pGrid->endX();
    int endY = pGrid->endY();
    double ds;
    double levelset;
    StateVector State;
    switch(dim){
        case 'x' :
            ds = pGrid->dx();
            // Solve flux
            pFlux->exec(dim,t,dt);
            // Explicit update formula -- Euler method
            for( int jj=startY; jj<endY; jj++){
                for( int ii=startX; ii<endX; ii++){
                    levelset = pGrid->levelset(ii,jj);
                    if( levelset > 0) continue;
                    pGrid->state(ii,jj) = pGrid->state(ii,jj) + (pGrid->flux(ii-1,jj)-pGrid->flux(ii,jj)) * dt/ds;
                }
            }
            break;
        case 'y' :
            ds = pGrid->dy();
            // Solve flux
            pFlux->exec(dim,t,dt);
            // Explicit update formula -- Euler method
            for( int jj=startY; jj<endY; jj++){
                for( int ii=startX; ii<endX; ii++){
                    levelset = pGrid->levelset(ii,jj);
                    if( levelset > 0) continue;
                    pGrid->state(ii,jj) = pGrid->state(ii,jj) + (pGrid->flux(ii,jj-1)-pGrid->flux(ii,jj)) * dt/ds;
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
    return;
}


double FVMsolverCutCell::getAlphaShielded( const BoundaryGeometry& Boundary, char dim){
    // Case 1
    // Cuts both sides
    // --------------
    // |            |
    // | __________/|
    // |/           |
    // |            |
    // --------------
    // In this case, alpha shielded is always going to be 0.5. Or average of betas.
    //
    // Case 2
    // Triangular cut
    // --------------
    // |  /         |
    // | /          |
    // |/           |
    // |            |
    // --------------
    // In this case, find average of opposite betas.
    //
    // Case 3
    // Cuts opposite sides
    // --------------
    // |      /     |
    // |     /      |
    // |    /       |
    // |   /        |
    // --------------
    // In this case, again find average of opposite betas
    //
    // We see that in all cases, alpha shielded is just the average of opposite betas.
    double beta1, beta2;
    switch(dim){
        case 'x':
            beta1 = Boundary.betaT();
            beta2 = Boundary.betaB();
            break;
        case 'y':
            beta1 = Boundary.betaL();
            beta2 = Boundary.betaR();
            break;
        default:
            // This code should never be called
            beta1=0.; beta2=0.;
            std::cerr << "ERROR: incorrect dim, cut cell alpha shielded" << std::endl;
            exit(EXIT_FAILURE);
    }
    return 0.5*(beta1+beta2);
}

//    FluxVector getShieldedFlux( int ii, int jj, char dim);
//    FluxVector getAuxFlux( int ii, int jj, char dim);
//    FluxVector getBoundaryFlux( int ii, int jj, char dim);

//FluxVector FVMsolverCutCell::getModifiedFlux( int ii, int jj, char dim){


}// Namespace closure
#endif /* FVMCUTCELL_HPP */
