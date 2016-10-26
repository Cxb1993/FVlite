// Solver.hpp
//
// Solver class. Solvers are tools which operate on
// Grids. Examples include finite volume solvers and finite
// difference solvers.
//
// The full solver is built using a variation on the decorator pattern
// in which the 'interface' class and the standard 'conrete' class are
// one and the same.

#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <libconfig.h++>

#include "Utils/ObjectFactory.hpp"
#include "Grid/Grid.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class Solver {
protected:
    Grid* mpGrid;
    Timer* mpTimer;
public:
    Solver(){}
    virtual ~Solver(){}

    virtual void init( Grid* pGrid, Timer* pTimer, Setting& cfg, Solver* p_solver = NULL);
    virtual void exec(){}
    virtual void newTimeStep(){}
};

// Declare factory

ObjectFactory<Solver> SolverFactory;

// function definitions

void Solver::init( Grid* pGrid, Timer* pTimer, Setting& cfg, Solver* p_solver){
    (void)p_solver;
    (void)cfg;
    mpGrid=pGrid;
    mpTimer=pTimer;
}


} // namespace closure
#endif // SOLVER_HPP
