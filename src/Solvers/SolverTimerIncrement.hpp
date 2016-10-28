// SolverTimerIncrement.hpp
//
// To be called at the end of each time step to signify that the system
// has advanced through time. One will be called automatically per cycle,
// without user input. Additional advances may be inserted by the user if
// a cycle spans multiple time-steps.

#ifndef SOLVERTIMERINCREMENT_HPP
#define SOLVERTIMERINCREMENT_HPP

#include "SolverDecorator.hpp"

namespace FVlite{

class SolverTimerIncrement : public SolverDecorator {
public:
    void exec();
};

// Register with factory

REGISTER( Solver, TimerIncrement)

// Function definitions

void SolverTimerIncrement::exec(){
    SolverDecorator::exec();
    mpTimer->advance();
    return;
}

} // namespace closure
#endif /* SOLVERTIMERINCREMENT_HPP */
