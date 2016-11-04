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
    virtual void exec();
};

// Register with factory

REGISTER( Operator, ExplicitUpdaterCutCells)

// Function defintions

void OperatorExplicitUpdaterCutCells::exec(){
    double dt = mpTimer->dt() * m_dt_ratio;
    double ds;
    int startX = mpGrid->startX();
    int startY = mpGrid->startY();
    int endX = mpGrid->endX();
    int endY = mpGrid->endY();
    // get offset start points
    int startXL = startX;
    int startYL = startY;
    switch(m_dim){
        case 'x' :
            ds = mpGrid->dx();
            startXL -= 1;
            break;
        case 'y' :
            ds = mpGrid->dy();
            startYL -= 1;
            break;
        case 'z' :
        default:
            ds = mpGrid->dx();
            startXL -= 1;
    }
    // Update
    StateVector State;
    BoundaryGeometry Boundary;
    FluxVector FluxL,FluxR,BoundaryFlux;
    double alpha, betaL, betaR;
    for( int jj = startY, jjL = startYL ; jj<endY; ++jj, ++jjL){
        for( int ii = startX, iiL = startXL; ii<endX; ++ii, ++iiL){
            Boundary = mpGrid->boundary(ii,jj);
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
            FluxL = mpGrid->flux(iiL,jjL);
            FluxR = mpGrid->flux(ii,jj);
            if( alpha == 1.){
                mpGrid->state(ii,jj) = mpGrid->state(ii,jj) + (FluxL-FluxR) * dt/ds;
            }else{
                BoundaryFlux.set( mpGrid->state_ref(ii,jj),m_dim);
                mpGrid->state(ii,jj) += (betaL*FluxL - betaR*FluxR - 
                    (betaL-betaR)*BoundaryFlux) * dt/(ds*alpha);
            }
        }
    }
    return;
}

}// Namespace closure
#endif /* OPERATOREXPLICITUPDATERCUTCELLS_HPP */
