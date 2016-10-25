// Event.hpp

#ifndef EVENT_HPP
#define EVENT_HPP

#include <cstdlib>
#include <string>
#include <exception>

#include <libconfig.h++>

#include "Utils/ObjectFactory.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

// Declare core objects, but don't define yet.

class Grid;
class Timer;
class Solver;

// The abstract 'Event' class contains nothing and does nothing -- it exists purely for the sake
// of abstraction, as there is not necessarily anything in common between different events.
// Templated over the type of object it controls (i.e. Grid, Timer, Solver).

class Event{
protected:
    Grid*   mpGrid;
    Timer*  mpTimer;
    Solver* mpSolver;
public:
    virtual ~Event(){}
    virtual void init( Grid* pGrid, Timer* pTimer, Solver* pSolver, Setting& cfg){
        (void)cfg;
        mpGrid   = pGrid;
        mpTimer  = pTimer;
        mpSolver = pSolver;
    }
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


}//namespace closure
#endif // EVENT_HPP
