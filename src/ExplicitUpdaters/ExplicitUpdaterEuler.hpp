// ExplicitUpdaterEuler.hpp
//
// Finite volume explicit update calculator.
// Uses Euler method to march forward in time.
// Note that 'Euler' refers to the approximate method
// for solving ODEs, not the Euler equations!

#ifndef EXPLICITUPDATEREULER_HPP
#define EXPLICITUPDATEREULER_HPP

#include <cstdlib>
#include <utility>
#include <iostream>
#include <string>

#include <libconfig.h++>

#include "ExplicitUpdaters/ExplicitUpdater.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class ExplicitUpdaterEuler : public ExplicitUpdater {
    virtual void exec( char dim, double dt);
};

// Register with factory

REGISTER( ExplicitUpdater, Euler)

// Function defintions

void ExplicitUpdaterEuler::exec( char dim, double dt){
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
    // Update
    for( int jj = startY, jjL = startYL ; jj<endY; ++jj, ++jjL){
        for( int ii = startX, iiL = startXL; ii<endX; ++ii, ++iiL){
            if( mpGrid->levelset(ii,jj) > 0) continue;
            mpGrid->state(ii,jj) = mpGrid->state(ii,jj) +
                (mpGrid->flux(iiL,jjL)-mpGrid->flux(ii,jj)) * dt/ds;
        }
    }
    return;
}

}// Namespace closure
#endif /* EXPLICITUPDATEREULER_HPP */
