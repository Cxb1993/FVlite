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
    virtual void exec( Grid& grid, Timer& timer);
};

// Register with factory

REGISTER( Operator, ExplicitUpdaterEuler)

// Function defintions

void OperatorExplicitUpdaterEuler::exec( Grid& grid, Timer& timer){
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
    for( int jj = startY, jjL = startYL ; jj<endY; ++jj, ++jjL){
        for( int ii = startX, iiL = startXL; ii<endX; ++ii, ++iiL){
            if( grid.levelset(ii,jj) > 0) continue;
            grid.state(ii,jj) = grid.state(ii,jj) +
                (grid.flux(iiL,jjL)-grid.flux(ii,jj)) * dt/ds;
        }
    }
    return;
}

}// Namespace closure
#endif /* OPERATOREXPLICITUPDATEREULER_HPP */
