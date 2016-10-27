// ExplicitUpdaterCutCell.hpp
//
// Finite volume explicit update calculator.
// Variation on ExplicitUpdaterEuler which accounts
// for presence of cut cells.

#ifndef EXPLICITUPDATERCUTCELLS_HPP
#define EXPLICITUPDATERCUTCELLS_HPP

#include <cstdlib>
#include <utility>
#include <iostream>
#include <string>

#include <libconfig.h++>

#include "ExplicitUpdaters/ExplicitUpdater.hpp"
#include "CutCellManager/CutCellManager.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class ExplicitUpdaterCutCells : public ExplicitUpdater {
protected:
    CutCellManager* mpCutCell;
public:
    virtual ~ExplicitUpdaterCutCells();
    virtual void init( Grid* pGrid, Timer* pTimer, Setting& cfg);
    virtual void exec( char dim, double dt);
};

// Register with factory

REGISTER( ExplicitUpdater, CutCells)

// Function defintions

ExplicitUpdaterCutCells::~ExplicitUpdaterCutCells(){
    delete mpCutCell;
}

void ExplicitUpdaterCutCells::init( Grid* pGrid, Timer* pTimer, Setting& cfg){
    ExplicitUpdater::init(pGrid,pTimer,cfg);
    mpCutCell = new CutCellManager;
    mpCutCell->init(pGrid,cfg);
}

void ExplicitUpdaterCutCells::exec( char dim, double dt){
    double ds;
    int startX = mpGrid->startX();
    int startY = mpGrid->startY();
    int endX = mpGrid->endX();
    int endY = mpGrid->endY();
    // get offset start points
    int startXL = startX;
    int startYL = startY;
    switch(dim){
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
    // Apply cut cell corrections
    mpCutCell->correctFluxes( dim, dt);
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
            betaL = Boundary.betaL();
            betaR = Boundary.betaR();
            FluxL = mpGrid->flux(iiL,jjL);
            FluxR = mpGrid->flux(ii,jj);
            if( alpha == 1.){
                mpGrid->state(ii,jj) = mpGrid->state(ii,jj) + (FluxL-FluxR) * dt/ds;
            }else{
                BoundaryFlux.set( mpGrid->state_ref(ii,jj),dim);
                mpGrid->state(ii,jj) += (betaL*FluxL - betaR*FluxR - 
                    (betaL-betaR)*BoundaryFlux) * dt/(ds*alpha);
            }
        }
    }
    return;
}

}// Namespace closure
#endif /* EXPLICITUPDATERCUTCELLS_HPP */
