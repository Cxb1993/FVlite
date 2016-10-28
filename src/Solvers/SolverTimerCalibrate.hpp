// SolverTimerCalibrate.hpp
//
// To be called at the start of each time step to account for changes in the
// maximum speed on the grid. One will be called automatically per cycle,
// without user input. Additional calibrations may be inserted by the user if
// desired.

#ifndef SOLVERTIMERCALIBRATE_HPP
#define SOLVERTIMERCALIBRATE_HPP

#include "SolverDecorator.hpp"

namespace FVlite{

class SolverTimerCalibrate : public SolverDecorator {
public:
    void exec();
};

// Register with factory

REGISTER( Solver, TimerCalibrate)

// Function definitions

void SolverTimerCalibrate::exec(){
    SolverDecorator::exec();
#ifndef MAXWELL /* No need to calibrate if max-speed is constant */
    mpTimer->calibrate();
#endif
    return;
}

} // namespace closure
#endif /* SOLVERTIMERCALIBRATE_HPP */
