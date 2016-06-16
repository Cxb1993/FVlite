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
#include "Limiters/Limiters.hpp"

#include "BasicFluxFunctions.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class FluxSolver{

protected:

    Grid*           pGrid;
    Limiter*        pLimiter;

public:

    FluxSolver(){}
    virtual ~FluxSolver();

    void init( Grid* pGrid, Setting& cfg);

    virtual void exec(char dim, double dt) = 0;
    virtual int stencilSize() = 0;
    virtual FluxVector getIntercellFlux( double ds, double dt, char dim, const StateVector& L, const StateVector& R) = 0;

};

// Factory declaration

ObjectFactory<FluxSolver> FluxSolverFactory;


// Function defintions

void FluxSolver::init( Grid* pGrid, Setting& cfg){
    string limitType = cfg.lookup("limiter");
    pLimiter = LimiterFactory.create(limitType);
    (*this).pGrid = pGrid;
    return;
}


FluxSolver::~FluxSolver(){
    delete pLimiter;
}


}// Namespace closure
#endif /* FLUXABSTRACT_HPP */
