// BoundaryUpdaterAbstract.hpp
//
// Abstract finite volume grid boundary updater.
//
// EDIT: Now contains general boundary functions for two states

#ifndef BUPDATERABSTRACT_HPP
#define BUPDATERABSTRACT_HPP

#include "Vectors/Grid.hpp"
#include <iostream>

namespace FVlite{

class BoundaryUpdater{

protected:

    Grid* pGrid;

public:

    BoundaryUpdater(){}
    virtual ~BoundaryUpdater(){}

    void init( Grid* pGrid);
    virtual void exec() = 0;
};

// Factory declaration

ObjectFactory<BoundaryUpdater> BoundaryUpdaterFactory;


// Function definitions

void BoundaryUpdater::init( Grid* pGrid){
    (*this).pGrid = pGrid;
    return;
}


// General boundary functions.
// Not associated with any class.

StateVector BoundaryReflective( const StateVector& Reference, const char dim){
    StateVector Boundary;
#ifdef MAXWELL
    Boundary = Reference;
    switch(dim){
        case 'x':
            Boundary[1] = 0;
            Boundary[2] = 0;
            break;
        case 'y':
            Boundary[0] = 0;
            Boundary[2] = 0;
            break;
        case 'z':
            break;
        default:
            std::cerr << "INCORRECT FLUX DIRECTION, BOUNDARY FUNCTION" << std::endl;
    }
#endif
#ifdef EULER
    Boundary = Reference;
    switch(dim){
        case 'x':
            Boundary[1] = -1.*Reference[1];
            break;
        case 'y':
            Boundary[2] = -1.*Reference[2];
            break;
        case 'z':
            break;
        default:
            std::cerr << "INCORRECT FLUX DIRECTION, BOUNDARY FUNCTION" << std::endl;
    }
#endif
    return Boundary;
}


}// Namespace closure
#endif /* BUPDATERABSTRACT_HPP */
