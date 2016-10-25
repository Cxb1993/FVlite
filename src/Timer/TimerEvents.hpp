// TimerEvents.hpp
//
// Defines Events which can control Timer operations

#include "EventSequencer/Event.hpp"
#include "Timer.hpp"

#ifndef TIMEREVENTS_HPP
#define TIMEREVENTS_HPP

namespace FVlite{

// The TimerIncrement Event should be called at the end of each timestep to advance time forward.

class EventTimerIncrement: public Event {
protected:
    double mRatio;
public:
    virtual void init( Grid* pGrid, Timer* pTimer, Solver* pSolver, Setting& cfg){
        Event::init(pGrid,pTimer,pSolver,cfg);
        try{
            mRatio = cfg.lookup("ratio");
        } catch ( const std::exception& e){
            mRatio = 1.0;
        }
    }
    virtual void exec(){
        mpTimer->advance(mRatio);
    }
};

REGISTER(Event,TimerIncrement)

// The TimerUpdate Event should be called at the start of each timestep to calibrate the time step
// for the following cycle.

class EventTimerCalibrate: public Event {
public:
    virtual void exec(){
        mpTimer->calibrate_timestep();
    }
};

REGISTER(Event,TimerCalibrate)

} // namespace closure
#endif // TIMEREVENTS_HPP
