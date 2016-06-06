// PDEsolverStd.hpp
//
// Standard finite volume PDE solver.
// Time marches using Euler method.

#ifndef FVMSTANDARD_HPP
#define FVMSTANDARD_HPP

#include <iostream>
#include <cstdlib>

#include "FVMsolverAbstract.hpp"

namespace FVlite{

class FVMsolverStd : public FVMsolver{

public:

    FVMsolverStd(){}
    virtual void exec( char dim, double t, double dt);
    virtual void newTimeStep(){/*do nothing*/}
};

REGISTER(FVMsolver,Std)

void FVMsolverStd::exec( char dim, double t, double dt){
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


}// Namespace closure
#endif /* FVMSTANDARD_HPP */
