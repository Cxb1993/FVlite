// Operator.hpp
//
// Operator class. Operators are tools which operate on
// Grids. Examples include finite volume solvers and finite
// difference solvers.
//
// The full solver is built using the 'composite' design pattern. A
// single Composite<Operator> is build which updates the grid a single
// timestep. This Composite contains a vector of Operators, which
// themselves may be Composite<Operator>.

#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include <libconfig.h++>

#include "Utils/ObjectFactory.hpp"
#include "Grid/Grid.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class Operator {
protected:
    Grid* mpGrid;
    Timer* mpTimer;
public:
    Operator(){}
    virtual ~Operator(){}

    virtual void init( Grid* pGrid, Timer* pTimer, Setting& cfg);
    virtual void exec() = 0;
};

// Declare factory

ObjectFactory<Operator> OperatorFactory;

// function definitions

void Operator::init( Grid* pGrid, Timer* pTimer, Setting& cfg){
    (void)cfg;
    mpGrid=pGrid;
    mpTimer=pTimer;
}

} // namespace closure
#endif // OPERATOR_HPP
