// ExplicitUpdater.hpp
//
// Finite volume explicit update calculator.
// Abstract version.

#ifndef EXPLICITUPDATER_HPP
#define EXPLICITUPDATER_HPP

#include "Grid/Grid.hpp"

namespace FVlite{

class ExplicitUpdater {
protected:
    Grid* mpGrid;
public:
    ExplicitUpdater(){}
    virtual ~ExplicitUpdater(){}

    virtual void init( Grid* pGrid);
    virtual void exec( char dim, double dt, double ds, int ii, int jj) = 0;
};

// Create factory

ObjectFactory<ExplicitUpdater> ExplicitUpdaterFactory;

// Function definitions

void ExplicitUpdater::init( Grid* pGrid){
    mpGrid = pGrid;
}

} // namespace closure
#endif // EXPLICITUPDATER_hpp
