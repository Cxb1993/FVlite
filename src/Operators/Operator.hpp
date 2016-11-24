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
#include "Timer/Timer.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class Operator {
public:
    Operator(){}
    virtual ~Operator(){}
    virtual void init( Setting& cfg){ (void)cfg; }
    virtual void exec( Grid& grid, Timer& timer) = 0;
};

// Declare factory

ObjectFactory<Operator> OperatorFactory;

} // namespace closure
#endif // OPERATOR_HPP
