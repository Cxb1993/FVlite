// BoundaryUpdaterTransmissive.hpp
//
// Finite volume grid boundary updater.
// Transmissive boundary conditions.

#ifndef BUPDATERTRANSMISSIVE_HPP
#define BUPDATERTRANSMISSIVE_HPP

#include"BoundaryUpdaterAbstract.hpp"

namespace FVTD{

class BoundaryUpdaterTransmissive : public BoundaryUpdater{
public:
    BoundaryUpdaterTransmissive(){}
    virtual void exec();
};

REGISTER(BoundaryUpdater,Transmissive)

void BoundaryUpdaterTransmissive::exec(){
    int bound  = pGrid->bound();
    int startX = pGrid->startX();
    int startY = pGrid->startY();
    int endX   = pGrid->endX();
    int endY   = pGrid->endY();
    // Left and right boundaries
    for( int ii=0; ii<bound; ii++){
        for( int jj=startY; jj<endY; jj++){
            pGrid->state(startX-1-ii,jj) = pGrid->state(startX+ii,jj);
            pGrid->state(endX+ii,jj)     = pGrid->state(endX-1-ii,jj);
        }
    }
    // Lower and upper boundaries
    for( int ii=startX; ii<endX; ii++){
        for( int jj=0; jj<bound; jj++){
            pGrid->state(ii,startY-1-jj) = pGrid->state(ii,startY+jj);
            pGrid->state(ii,endY+jj)     = pGrid->state(ii,endY-1-jj);
        }
    }

    return;
}

}// Namespace closure
#endif /* BUPDATERTRANSMISSIVE_HPP */
