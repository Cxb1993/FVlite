// OperatorTimerIncrement.hpp
//
// To be called at the end of each time step to signify that the system
// has advanced through time. One will be called automatically per cycle,
// without user input. Additional advances may be inserted by the user if
// a cycle spans multiple time-steps.

#ifndef OPERATORTIMERINCREMENT_HPP
#define OPERATORTIMERINCREMENT_HPP

#include "Operator.hpp"

namespace FVlite{

class OperatorTimerIncrement : public Operator {
public:
    void exec();
};

// Register with factory

REGISTER( Operator, TimerIncrement)

// Function definitions

void OperatorTimerIncrement::exec(){
    mpTimer->advance();
    return;
}

} // namespace closure
#endif /* OPERATORTIMERINCREMENT_HPP */
