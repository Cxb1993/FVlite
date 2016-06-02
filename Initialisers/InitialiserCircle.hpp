// InitialiserCirlce.hpp
//
// Finite volume grid initialisaer.
// Initialises zeroes except for PEC cylinder in center of grid.

#ifndef INITIALISERCIRCLE_HPP
#define INITIALISERCIRCLE_HPP

#include"InitialiserAbstract.hpp"

#include <cmath>

namespace FVTD{

class InitialiserCircle : public Initialiser{
public:
    InitialiserCircle( Grid* pGrid) : Initialiser(pGrid) {}
    virtual void exec();
};

void InitialiserCircle::exec(){

    double x, y;
    double rad = pGrid->Lx()/8.;
    double centerX = 0.5;
    double centerY = 0.5;
    double distance;

#ifdef EULER
    StateVector LEFT( 1.0, 0.75, 0.0, 1.0);
    StateVector RIGHT( 0.125, 0.0, 0.0, 0.1);
    double left_pos = 0.25;
#endif

    int startX = pGrid->startX();
    int endX = pGrid->endX();
    int startY = pGrid->startY();
    int endY = pGrid->endY();
    int sizeX = pGrid->Nx() + 2*pGrid->bound();
    int sizeY = pGrid->Ny() + 2*pGrid->bound();

    for( int jj=0; jj<sizeY; jj++){
        y = pGrid->y(jj);
        for( int ii=0; ii<sizeX; ii++){
            x = pGrid->x(ii);
            if( ii>=startX && ii<endX && jj>=startY && jj<endY){
#ifdef EULER
                if( x < left_pos){
                    pGrid->state(ii,jj) = LEFT;
                } else {
                    pGrid->state(ii,jj) = RIGHT;
                }
#else
                pGrid->state(ii,jj)    = ZEROSTATE;
#endif
            }
            // Level set -- signed distance function
            distance = sqrt((x-centerX)*(x-centerX)+(y-centerY)*(y-centerY));
            pGrid->levelset()->workspace(ii,jj) = rad - distance;
        }
    }
    pGrid->levelset()->merge();
    return;
}

}// Namespace Closure
#endif /* INITIALISERCIRCLE_HPP */
