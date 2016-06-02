// InitialiserExplosiveTest.hpp
//
// Finite volume grid initialisaer.
// Initialises with a circular high-density and high-pressure region in center of grid.

#ifndef INITIALISEREXPLOSIVETEST_HPP
#define INITIALISEREXPLOSIVETEST_HPP

#ifdef EULER

#include"InitialiserAbstract.hpp"

#include <iostream>
#include <cmath>

namespace FVTD{

class InitialiserExplosiveTest : public Initialiser{
public:
    InitialiserExplosiveTest(){}
    virtual void exec();
};

REGISTER(Initialiser,ExplosiveTest)

void InitialiserExplosiveTest::exec(){

    double x, y;
    double rad = 0.4;
    double centerX = 1.0;
    double centerY = 1.0;
    double distance;

    StateVector INNER( 1., 0., 0., 1.);
    StateVector OUTER( 0.125, 0., 0., 0.1);

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
                distance = sqrt((x-centerX)*(x-centerX)+(y-centerY)*(y-centerY));
                if( distance < rad ){
                    pGrid->state(ii,jj) = INNER;
                } else {
                    pGrid->state(ii,jj) = OUTER;
                }
            }
        }
    }
    return;
}

}// Namespace Closure

#endif /* EULER */
#endif /* INITIALISEREXPLOSIVETEST_HPP */
