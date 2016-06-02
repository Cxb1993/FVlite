// InitialiserRectangle.hpp
//
// Finite volume grid initialisaer.
// Initialises zeroes except for PEC rectangle in center of grid.

#ifndef INITIALISERRECTANGLE_HPP
#define INITIALISERRECTANGLE_HPP

#include"InitialiserAbstract.hpp"

#include <cmath>

namespace FVTD{

class InitialiserRectangle : public Initialiser{
public:
    InitialiserRectangle( Grid* pGrid) : Initialiser(pGrid) {}
    virtual void exec();
};

void InitialiserRectangle::exec(){

    // Uses lower-left (LL) and upper-right (UR) coordinates to determine position

    double x, y;
    double LLx = 0.4;
    double LLy = 0.4;
    double URx = 0.6;
    double URy = 0.6;
    double levelset = 0.; // Compiler complains this might not be initialised otherwise. Not sure why.
    int bound = pGrid->bound();
    double dx = pGrid->dx();
    double dy = pGrid->dy();

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
        y = (jj-bound+0.5)*dy;
        for( int ii=0; ii<sizeX; ii++){
            x = (ii-bound+0.5)*dx;
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
            // Level set. Check several areas of grid
            // Left side
            if( x < LLx){
                // Lower
                if( y < LLy){
                    levelset = -sqrt( (x-LLx)*(x-LLx) + (y-LLy)*(y-LLy));
                }
                // Central
                if( y >= LLy && y <= URy){
                    levelset = x - LLx;
                }
                // Upper
                if( y > URy){
                    levelset = -sqrt( (x-LLx)*(x-LLx) + (y-URy)*(y-URy));
                }
            }
            // Central region
            if( x >= LLx && x <= URx){
                // Lower
                if( y < LLy){
                    levelset = y - LLy;
                }
                // Central
                if( y >= LLy && y <= URy){
                    levelset = fmin( x-LLx, URx-x);
                    levelset = ( fmin( y-LLy, URy-y) < levelset) ? fmin(y-LLy,URy-y) : levelset;
                }
                // Upper
                if( y > URy){
                    levelset = URy - y;
                }
            }
            // Right side
            if( x > URx){
                // Lower
                if( y < LLy){
                    levelset = -sqrt( (x-URx)*(x-URx) + (y-LLy)*(y-LLy));
                }
                // Central
                if( y >= LLy && y <= URy){
                    levelset = URx - x;
                }
                // Upper
                if( y > URy){
                    levelset = -sqrt( (x-URx)*(x-URx) + (y-URy)*(y-URy));
                }
            }
            pGrid->levelset()->workspace(ii,jj) = levelset;
        }
    }

    pGrid->levelset()->merge();

    return;
}

}// Namespace Closure
#endif /* INITIALISERRECTANGLE_HPP */
