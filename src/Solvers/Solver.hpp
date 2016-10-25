// Solver.hpp
//
// Abstract Solver class. Solvers are tools which operate on
// Grids. Examples include finite volume solvers and finite
// difference solvers.

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
public:
    Solver(){}
    virtual ~Solver(){}

    virtual void init( Grid* pGrid, Setting& cfg);
    virtual void exec( char dim, double dt) = 0;
    virtual void newTimeStep(){}
};

// declare factory

ObjectFactory<Solver> SolverFactory;

// function definitions

void Solver::init( Grid* pGrid, Setting& cfg){
    (void)cfg;
    mpGrid=pGrid;
}


} // namespace closure
#endif // SOLVER_HPP

