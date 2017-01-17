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
    unsigned int sweep_dim = (m_dim=='x') ? DIM_X : DIM_Y;
    unsigned int startX = grid.state_start(DIM_X);
    unsigned int startY = grid.state_start(DIM_Y);
    unsigned int endX = grid.state_end(DIM_X);
    unsigned int endY = grid.state_end(DIM_Y);
    unsigned int f_startX = grid.flux_start(DIM_X);
    unsigned int f_startY = grid.flux_start(DIM_Y);
    double ds = grid.ds(sweep_dim);
    // Update
    for( unsigned int jj = startY, jjF = f_startY ; jj<endY; ++jj, ++jjF){
        for( unsigned int ii = startX, iiF = f_startX; ii<endX; ++ii, ++iiF){
            if( grid.approx_central_levelset(ii,jj) > 0) continue;
            grid.state(ii,jj) += (grid.flux(iiF,jjF)-grid.flux(iiF+(sweep_dim==DIM_X),jjF+(sweep_dim==DIM_Y))) * dt/ds;
        }
    }
    return;
}

}// Namespace closure
#endif /* OPERATOREXPLICITUPDATEREULER_HPP */
