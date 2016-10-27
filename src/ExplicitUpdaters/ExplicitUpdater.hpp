// ExplicitUpdater.hpp
//
// Finite volume explicit update calculator.
// Abstract version.

#ifndef EXPLICITUPDATER_HPP
#define EXPLICITUPDATER_HPP

#include <libconfig.h++>

#include "Grid/Grid.hpp"
#include "Timer/Timer.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class ExplicitUpdater {
protected:
    Grid* mpGrid;
    Timer* mpTimer;
public:
    ExplicitUpdater(){}
    virtual ~ExplicitUpdater(){}

    virtual void init( Grid* pGrid, Timer* pTimer, Setting& cfg);
    virtual void exec( char dim, double dt) = 0;
};

// Create factory

ObjectFactory<ExplicitUpdater> ExplicitUpdaterFactory;

// Function definitions

void ExplicitUpdater::init( Grid* pGrid, Timer* pTimer, Setting& cfg){
    (void)cfg;
    mpGrid = pGrid;
    mpTimer = pTimer;
}

} // namespace closure
#endif // EXPLICITUPDATER_hpp
