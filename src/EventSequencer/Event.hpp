// Event.hpp

#ifndef EVENT_HPP
#define EVENT_HPP

#include <cstdlib>
#include <string>
#include <exception>

#include <libconfig.h++>

#include "Utils/ObjectFactory.hpp"
#include "Grid/Grid.hpp"
#include "Timer/Timer.hpp"
#include "Solver/Solver.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

// The abstract 'Event' class contains nothing and does nothing -- it exists purely for the sake
// of abstraction, as there is not necessarily anything in common between different events.

class Event{
public:
    virtual ~Event(){}
    virtual void init( Grid* pGrid, Timer* pTimer, Solver* pSolver, Setting& cfg) = 0;
    virtual void exec() = 0;
};

// Setting up event factory

ObjectFactory<Event> EventFactory;

// EventExceptions should be defined for each Event. This allows code failures to be diagnosed
// more rapidly.

class EventException: public std::exception {
    virtual const char* what() const throw(){
        return "Code failure during unspecified event.";
    }
};

// The TimerIncrement Event should be called at the end of each timestep to advance time forward.

class EventTimerIncrement: public Event {
protected:
    Timer* mpTimer;
public:
    virtual void init( Grid* pGrid, Timer* pTimer, Solver* pSolver, Setting& cfg){
        (void)pGrid;
        (void)pSolver;
        (void)cfg;
        mpTimer = pTimer;
    }
    virtual void exec(){
        mpTimer->advance();
    }
};

REGISTER(Event,TimerIncrement)

// The TimerUpdate Event should be called at the start of each timestep to calibrate the time step
// for the following cycle.

class EventTimerUpdate: public Event {
protected:
    Timer* mpTimer;
public:
    virtual void init( Grid* pGrid, Timer* pTimer, Solver* pSolver, Setting& cfg){
        (void)pGrid;
        (void)pSolver;
        (void)cfg;
        mpTimer = pTimer;
    }
    virtual void exec(){
        mpTimer->calibrate_timestep();
    }
};

REGISTER(Event,TimerUpdate)

}//namespace closure
#endif // EVENT_HPP
