// BoundaryUpdaterConstant.hpp
//
// Finite volume grid boundary updater.
// Constant boundary conditions.

#ifndef BUPDATERCONSTANT_HPP
#define BUPDATERCONSTANT_HPP

#include"BoundaryUpdaterAbstract.hpp"

namespace FVlite{

class BoundaryUpdaterConstant : public BoundaryUpdater{
public:
    BoundaryUpdaterConstant(){}
    virtual void exec();
};

REGISTER(BoundaryUpdater,Constant)

void BoundaryUpdaterConstant::exec(){
    static bool boundariesSet = false;
    int bound  = pGrid->bound();
    int startX = pGrid->startX();
    int startY = pGrid->startY();
    int endX   = pGrid->endX();
    int endY   = pGrid->endY();
    if(!boundariesSet){
        // Left and right boundaries
        for( int ii=0; ii<bound; ii++){
            for( int jj=startY; jj<endY; jj++){
                pGrid->state(startX-1-ii,jj) = pGrid->state(startX,jj);
                pGrid->state(endX+ii,jj)     = pGrid->state(endX-1,jj);
            }
        }
        // Lower and upper boundaries
        for( int ii=startX; ii<endX; ii++){
            for( int jj=0; jj<bound; jj++){
                pGrid->state(ii,startY-1-jj) = pGrid->state(ii,startY);
                pGrid->state(ii,endY+jj)     = pGrid->state(ii,endY-1);
            }
        }
        boundariesSet = true;
    }
    return;
}

}// Namespace closure
#endif /* BUPDATERCONSTANT_HPP */
