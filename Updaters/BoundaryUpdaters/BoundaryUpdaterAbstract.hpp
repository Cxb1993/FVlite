// BoundaryUpdaterAbstract.hpp
//
// Abstract finite volume grid boundary updater.

#ifndef BUPDATERABSTRACT_HPP
#define BUPDATERABSTRACT_HPP

#include"Vectors/Grid.hpp"

namespace FVTD{

class BoundaryUpdater{

protected:

    Grid* pGrid;

public:

    BoundaryUpdater( Grid* pGrid);
    virtual ~BoundaryUpdater();

    virtual void exec() = 0;
};

BoundaryUpdater::BoundaryUpdater( Grid* pGrid) : pGrid(pGrid){
}

BoundaryUpdater::~BoundaryUpdater(){
}

}// Namespace closure
#endif /* BUPDATERABSTRACT_HPP */
