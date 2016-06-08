// FluxSolverAbstract.hpp
//
// Abstract finite volume flux solver.

#ifndef FLUXABSTRACT_HPP
#define FLUXABSTRACT_HPP

#include <iostream>
#include <cstdlib>
#include <string>
#include <libconfig.h++>

#include "Grid/Grid.hpp"
#include "Timer/Timer.hpp"
#include "Sources/Sources.hpp"
#include "Limiters/Limiters.hpp"

#include "BasicFluxFunctions.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class FluxSolver{

protected:

    Grid*           pGrid;
    Source*         pSource;
    Limiter*        pLimiter;

public:

    FluxSolver(){}
    virtual ~FluxSolver();

    void init( Grid* pGrid, Source* pSource, string LimitString);
    void init( Grid* pGrid, Source* pSource, Setting& cfg);
    void exec( char dim, double t, double dt);

    virtual void solve(char dim, double dt) = 0;
    virtual int stencilSize() = 0;
    virtual FluxVector getIntercellFlux( double ds, double dt, char dim, const StateVector& L, const StateVector& R) = 0;

    // DEPRECATED
    void inject_source( char dim, double t, double dt);

};

// Factory declaration

ObjectFactory<FluxSolver> FluxSolverFactory;


// Function defintions

void FluxSolver::init( Grid* pGrid, Source* pSource, string LimitString){
    (*this).pGrid = pGrid;
    (*this).pSource = pSource;
    pLimiter = LimiterFactory.create(LimitString);
}

void FluxSolver::init( Grid* pGrid, Source* pSource, Setting& cfg){
    string limitType = cfg.lookup("limiter");
    pLimiter = LimiterFactory.create(limitType);
    string cutCellType = cfg.lookup("cutcells");
    (*this).pGrid = pGrid;
    (*this).pSource = pSource;
    return;
}


FluxSolver::~FluxSolver(){
    delete pLimiter;
}

void FluxSolver::exec( char dim, double t, double dt){
    solve(dim,dt);
    inject_source(dim,t,dt);
    return;
}


// DEPRECATED

void FluxSolver::inject_source( char dim, double t, double dt){
#ifdef MAXWELL
    if( dim != 'x' ) return;
    StateVector Source, StateR;
    Source = pSource->exec(t);
    double ds = pGrid->dx();
    int startX = pGrid->startX();
    int endX   = pGrid->endX();
    int startY = pGrid->startY();
    int endY   = pGrid->endY();
    FluxVector Flux;
    (void)endX;
    for( int jj=startY; jj<endY; jj++){
        StateR = pGrid->state(startX,jj);
        Flux = Richtmyer_flux(ds,dt,dim,Source,StateR);
        pGrid->flux(startX-1,jj) = Flux;
    }
#else
    (void)dim;
    (void)t;
    (void)dt;
#endif
    return;
}


}// Namespace closure
#endif /* FLUXABSTRACT_HPP */
