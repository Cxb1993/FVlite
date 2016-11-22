// OperatorTimerIncrement.hpp
//
// To be called at the end of each time step to signify that the system
// has advanced through time. One will be called automatically per cycle,
// without user input. Additional advances may be inserted by the user if
// a cycle spans multiple time-steps.

#ifndef OPERATORTIMERINCREMENT_HPP
#define OPERATORTIMERINCREMENT_HPP

#include "Operators/Operator.hpp"

namespace FVlite{

class OperatorTimerIncrement : public Operator , public Visitable<OperatorTimerIncrement>{
public:
    void exec( Timer& timer);
};

// Register with factory

REGISTER( Operator, TimerIncrement)

// Function definitions

void OperatorTimerIncrement::exec( Timer& timer){
    timer.advance();
}

} // namespace closure
#endif /* OPERATORTIMERINCREMENT_HPP */
