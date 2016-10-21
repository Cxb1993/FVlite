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
