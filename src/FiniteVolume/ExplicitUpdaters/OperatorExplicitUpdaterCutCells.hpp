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
    int startX = grid.startX();
    int startY = grid.startY();
    int endX = grid.endX();
    int endY = grid.endY();
    // get offset start points
    int startXL = startX;
    int startYL = startY;
    switch(m_dim){
        case 'x' :
            ds = grid.dx();
            startXL -= 1;
            break;
        case 'y' :
            ds = grid.dy();
            startYL -= 1;
            break;
        case 'z' :
        default:
            ds = grid.dx();
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
                BoundaryFlux.set( grid.state_ref(ii,jj),m_dim);
                grid.state(ii,jj) += (betaL*FluxL - betaR*FluxR - 
                    (betaL-betaR)*BoundaryFlux) * dt/(ds*alpha);
            }
        }
    }
    return;
}

}// Namespace closure
#endif /* OPERATOREXPLICITUPDATERCUTCELLS_HPP */
