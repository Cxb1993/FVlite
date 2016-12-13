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
    double ds;
    int startX = grid.start(DIM_X);
    int startY = grid.start(DIM_Y);
    int endX = grid.end(DIM_X);
    int endY = grid.end(DIM_Y);
    // get offset start points
    int startXL = startX;
    int startYL = startY;
    switch(m_dim){
        case 'x' :
            ds = grid.ds(DIM_X);
            startXL -= 1;
            break;
        case 'y' :
            ds = grid.ds(DIM_Y);
            startYL -= 1;
            break;
        case 'z' :
        default:
            ds = grid.ds(DIM_X);
            startXL -= 1;
    }
    // Update
    StateVector State;
    BoundaryGeometry Boundary;
    FluxVector FluxL,FluxR,BoundaryFlux;
    double alpha, betaL, betaR;
    for( int jj = startY, jjL = startYL ; jj<endY; ++jj, ++jjL){
        for( int ii = startX, iiL = startXL; ii<endX; ++ii, ++iiL){
            Boundary = grid.boundary(ii,jj);
            alpha = Boundary.alpha();
            if( alpha == 0.) continue;
            // TODO get this switch out of here!
            switch( m_dim){
                case 'x':
                    betaL = Boundary.betaL();
                    betaR = Boundary.betaR();
                    break;
                case 'y':
                    betaL = Boundary.betaB();
                    betaR = Boundary.betaT();
                    break;
                default:
                    betaL = Boundary.betaL();
                    betaR = Boundary.betaR();
            }
            FluxL = grid.flux(iiL,jjL);
            FluxR = grid.flux(ii,jj);
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
