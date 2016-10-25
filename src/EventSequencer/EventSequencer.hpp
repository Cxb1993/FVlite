// EventSequencer.hpp
//
// A class containing a list of 'Events', defined as anything which reads or writes from the grid.
// This can include a directional sweep, a boundary update, printing to file, updating mixed cells,
// using an absorbing boundary layer, calculating source terms, etc.

#ifndef EVENTSEQUENCER_HPP
#define EVENTSEQUENCER_HPP

#include <cstdlib>
#include <iostream>
#include <vector>

#include <libconfig.h++>

#include "Grid/Grid.hpp"
#include "Timer/Timer.hpp"
#include "Solvers/Solver.hpp"

using std::stringl
using libconfig::Setting;

namespace FVLite{

class EventSequencer {

protected:

    std::vector<Event*> mEvents;

public:

    EventSequencer(){}
    ~EventSequencer(){
        for( std::vector<Event*>::iterator It = mEvents.begin(); It != mEvents.end(); ++It){
            delete *It;
        }
    }

    // Build up a sequence of events from config file
    // Must pass in Setting named "Events"
    void init( Grid* pGrid, Timer* pTimer, Solver* pSolver, Setting& cfg){
        mpGrid
        int nEvents = cfg.getLength();
        for( int count=0; count<nEvents; count++){
            Setting& eventCfg = cfg[count];
            string eventType = eventCfg.lookup("type").c_str();
            Event* pEvent = EventFactory.create( eventType);
            pEvent->init( pGrid, pTimer, pSolver, eventCfg);
            add_event( pEvent);
        }
    }

    // Cycle through all events in sequence
    void cycle(){
        int err;
        for( std::vector<Event*>::iterator It = mEvents.begin(); It != mEvents.end(); ++It){
            try{
                *It->exec();
            } catch ( const std::exception& e){
                std::cerr << e.what() << std::endl;
            }
        }
    }

    // Add event at end of sequence
    void add_event( Event* pEvent){
        mEvents.push_back(pEvent);
    }
};

} // namespace closure
#endif // EVENTSEQUENCER_HPP
