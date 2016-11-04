// OperatorExplicitUpdaterEuler.hpp
//
// Finite volume explicit update calculator.
// Uses Euler method to march forward in time.
// Note that 'Euler' refers to the approximate method
// for solving ODEs, not the Euler equations!

#ifndef OPERATOREXPLICITUPDATEREULER_HPP
#define OPERATOREXPLICITUPDATEREULER_HPP

#include <cstdlib>
#include <utility>
#include <iostream>
#include <string>

#include <libconfig.h++>

#include "OperatorExplicitUpdater.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class OperatorExplicitUpdaterEuler : public OperatorExplicitUpdater {
    virtual void exec();
};

// Register with factory

REGISTER( Operator, ExplicitUpdaterEuler)

// Function defintions

void OperatorExplicitUpdaterEuler::exec(){
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
#endif /* OPERATOREXPLICITUPDATEREULER_HPP */
