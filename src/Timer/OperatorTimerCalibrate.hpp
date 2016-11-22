// OperatorTimerCalibrate.hpp
//
// To be called at the start of each time step to account for changes in the
// maximum speed on the grid. One will be called automatically per cycle,
// without user input. Additional calibrations may be inserted by the user if
// desired.

#ifndef OPERATORTIMERCALIBRATE_HPP
#define OPERATORTIMERCALIBRATE_HPP

#include "Operators/Operator.hpp"

namespace FVlite{

class OperatorTimerCalibrate : public Operator, public Visitable<OperatorTimerCalibrate>{
public:
    void exec();
};

// Register with factory

REGISTER( Operator, TimerCalibrate)

// Function definitions

void OperatorTimerCalibrate::exec( Timer& timer, double max_speed, double min_ds){
    timer.set_dt( min_ds * timer.cfl() / max_speed);
}

} // namespace closure
#endif /* SOLVERTIMERCALIBRATE_HPP */
