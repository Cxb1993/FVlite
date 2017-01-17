// OperatorExplicitUpdaterCutCell.hpp
//
// Finite volume explicit update calculator.
// Variation on OperatorExplicitUpdaterEuler which accounts
// for presence of cut cells.

#ifndef OPERATOREXPLICITUPDATERCUTCELLS_HPP
#define OPERATOREXPLICITUPDATERCUTCELLS_HPP

#include <cstdlib>
#include <utility>
#include <iostream>
#include <string>

#include <libconfig.h++>

#include "OperatorExplicitUpdater.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class OperatorExplicitUpdaterCutCells : public OperatorExplicitUpdater {
    virtual void exec( Grid& grid, Timer& timer);
};

// Register with factory

REGISTER( Operator, ExplicitUpdaterCutCells)

// Function defintions

void OperatorExplicitUpdaterCutCells::exec( Grid& grid, Timer& timer){
    double dt = timer.dt() * m_dt_ratio;
    unsigned int sweep_dim = (m_dim=='x') ? DIM_X : DIM_Y;
    unsigned int startX = grid.state_start(DIM_X);
    unsigned int startY = grid.state_start(DIM_Y);
    unsigned int endX = grid.state_end(DIM_X);
    unsigned int endY = grid.state_end(DIM_Y);
    unsigned int f_startX = grid.flux_start(DIM_X);
    unsigned int f_startY = grid.flux_start(DIM_Y);
    double ds = grid.ds(sweep_dim);
    // Update
    StateVector State;
    BoundaryGeometry Boundary;
    FluxVector FluxL,FluxR,BoundaryFlux;
    double alpha, betaL, betaR;
    for( unsigned int jj = startY, jjF = f_startY ; jj<endY; ++jj, ++jjF){
        for( unsigned int ii = startX, iiF = f_startX; ii<endX; ++ii, ++iiF){
            Boundary = grid.boundary(ii,jj);
            alpha = Boundary.alpha();
            if( alpha == 0.) continue;
            betaL = (m_dim==DIM_X) ? Boundary.betaL() : Boundary.betaB();
            betaR = (m_dim==DIM_X) ? Boundary.betaR() : Boundary.betaT();
            FluxL = grid.flux(iiF,jjF);
            FluxR = grid.flux(iiF+(sweep_dim==DIM_X),jjF+(sweep_dim==DIM_Y));
            if( alpha == 1.){
                grid.state(ii,jj) = grid.state(ii,jj) + (FluxL-FluxR) * dt/ds;
            }else{
                BoundaryFlux.set( grid.boundary_state(ii,jj), grid.material(ii,jj), m_dim);
                grid.state(ii,jj) += (betaL*FluxL - betaR*FluxR - 
                    (betaL-betaR)*BoundaryFlux) * dt/(ds*alpha);
            }
        }
    }
    return;
}

}// Namespace closure
#endif /* OPERATOREXPLICITUPDATERCUTCELLS_HPP */
