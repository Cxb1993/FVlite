// PDEsolverAbstract.hpp
//
// Abstract finite volume PDE solver.

#ifndef PDEABSTRACT_HPP
#define PDEABSTRACT_HPP

#include <iostream>
#include <cstdlib>

#include "Vectors/Grid.hpp"
#include "Limiters/Limiters.hpp"

namespace FVTD{

class PDEsolver{

protected:

    Grid*             pGrid;
    Limiter*          pLimiter;

public:

    PDEsolver( Grid* pGrid, LIMIT_TYPE LTYPE=NONE);
    virtual ~PDEsolver();

    virtual void updateFlux( char dim, double dt) = 0;
    void updateFluxAlt( char dim, double dt);
    void exec( char dim, double dt);
};

PDEsolver::PDEsolver( Grid* pGrid, LIMIT_TYPE LTYPE)
        : pGrid(pGrid){
    switch(LTYPE){
        case(SUPERBEE):
            pLimiter = new LimiterSuperBee;
            break;
        case(MINBEE):
            pLimiter = new LimiterMinBee;
            break;
        case(VANLEER):
            pLimiter = new LimiterVanLeer;
            break;
        case(NONE):
            pLimiter = new LimiterNone;
            break;
        default:
            pLimiter = new LimiterMinBee;
    }
}

PDEsolver::~PDEsolver(){
    delete pLimiter;
}

void PDEsolver::updateFluxAlt( char dim, double dt){
    pGrid->swapStateVectors(); // Put State in Alt, Alt in State
    updateFlux(dim,dt);        // Calculate flux
    pGrid->swapStateVectors(); // Return States to original locations
    return;
}

void PDEsolver::exec( char dim, double dt){
    // Uses predictor/corrector time stepping
    int startX = pGrid->startX();
    int startY = pGrid->startY();
    int endX = pGrid->endX();
    int endY = pGrid->endY();
    double ds;
    StateVector State;
    switch(dim){
        case 'x' :
            ds = pGrid->dx();
            // Predictor step: Find state at n+1/2, put in Alt storage
            updateFlux(dim,dt);
/*            for( int jj=startY; jj<endY; jj++){
                for( int ii=startX; ii<endX; ii++){
                    pGrid->stateAlt(ii,jj) = pGrid->state(ii,jj) + (pGrid->flux(ii-1,jj)-pGrid->flux(ii,jj)) * dt/(2.*ds);
                }
            }
            // Corrector step: Find flux of n+1/2 states. Use this to calculate n+1
            updateFluxAlt(dim,dt);
  */          for( int jj=startY; jj<endY; jj++){
                for( int ii=startX; ii<endX; ii++){
                    pGrid->state(ii,jj) = pGrid->state(ii,jj) + (pGrid->flux(ii-1,jj)-pGrid->flux(ii,jj)) * dt/ds;
                }
            }
            break;
        case 'y' :
            ds = pGrid->dy();
            // Predictor step:
            updateFlux(dim,dt);
/*            for( int ii=startX; ii<endX; ii++){
                for( int jj=startY; jj<endY; jj++){
                    pGrid->stateAlt(ii,jj) = pGrid->state(ii,jj) + (pGrid->flux(ii,jj-1)-pGrid->flux(ii,jj)) * dt/(2.*ds);
                }
            }
            // Corrector step:
            updateFluxAlt(dim,dt);
  */          for( int ii=startX; ii<endX; ii++){
                for( int jj=startY; jj<endY; jj++){
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

// The following functions are used to build larger schemes

inline FluxVector LaxFriedrichs_flux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR, const Material& ML, const Material& MR){
    static FluxVector FL;
    static FluxVector FR;
    FluxVector result;
    FL.set(UL,ML,dim);
    FR.set(UR,MR,dim);
    result = 0.5*(FL+FR+(ds/dt)*(UL-UR));
    return result;
}

inline FluxVector Richtmyer_flux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR, const Material& ML, const Material& MR){
    // NOTE:
    // The Richtmyer scheme takes states U_i and U_i+1, and generates an intermediate state half of a time step forward at U_i+1/2
    // The flux calculated at the end depends on the material properties, which are undefined at position i+1/2
    // Here, we assume the intermediate state at U_i+1/2 has the same material properties as U_i+1
    static FluxVector FL;
    static FluxVector FR;
    static StateVector Inter;
    FluxVector result;
    FL.set(UL,ML,dim);
    FR.set(UR,MR,dim);
    Inter = 0.5*(UL+UR+(dt/ds)*(FL-FR));
    result.set(Inter,MR,dim);
    return result;
}

inline FluxVector FORCE_flux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR, const Material& ML, const Material& MR){
    FluxVector result;
    result = 0.5*(LaxFriedrichs_flux(ds,dt,dim,UL,UR,ML,MR)+Richtmyer_flux(ds,dt,dim,UL,UR,ML,MR));
    return result; 
}

}// Namespace closure
#endif /* PDEABSTRACT_HPP */
