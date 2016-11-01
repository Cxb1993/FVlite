// SolverDecorator.hpp
//
// SolverDecorator class. Allows solver to be specified at run time
// by recursively wrapping a core solver with new operations.
//
// The full solver is built using a variation on the decorator pattern
// in which the 'interface' class and the standard 'conrete' class are
// one and the same.

#ifndef SOLVERDECORATOR_HPP
#define SOLVERDECORATOR_HPP

#include <libconfig.h++>

#include "Solver.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class SolverDecorator : public Solver {
protected:
    Solver* mp_wrapped;
public:
    SolverDecorator(){}
    virtual ~SolverDecorator(){
        delete mp_wrapped;
    }

    virtual void init( Grid* pGrid, Timer* pTimer, Setting& cfg, Solver* p_wrapped){
        Solver::init(pGrid,pTimer,cfg);
        mp_wrapped = p_wrapped;
    }
    
    virtual void exec(){
        mp_wrapped->exec();
    }
};

// Note: do not register with factory! Should never need to build this directly.

} // namespace closure
#endif // SOLVERDECORATOR_HPP
