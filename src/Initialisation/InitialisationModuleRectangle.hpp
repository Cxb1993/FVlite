// BoundaryModuleRectangle.hpp
//
// Initialises with a rectangular pattern.
// Alligns with grid.

#ifndef INITIALISATIONMODULERECTANGLE_HPP
#define INITIALISATIONMODULERECTANGLE_HPP

#include <cmath>
#include <libconfig.h++>

#include "InitialisationModuleAbstract.hpp"

using libconfig::Setting;

namespace FVlite{

class InitialisationModuleRectangle : public InitialisationModule{

protected:

    double mLowerLeftX;
    double mLowerLeftY;
    double mUpperRightX;
    double mUpperRightY;

public:

    InitialisationModuleRectangle(){}
    virtual void init( Setting& cfg);
    virtual double exec( double x, double y);

};

REGISTER(InitialisationModule,Rectangle)


void InitialisationModuleRectangle::init( Setting& cfg){
    mLowerLeftX = cfg.lookup("lowerleft.x");
    mLowerLeftY = cfg.lookup("lowerleft.y");
    mUpperRightX = cfg.lookup("upperright.x");
    mUpperRightY = cfg.lookup("upperright.y");
    return;
}


double InitialisationModuleRectangle::exec( double x, double y){
    double levelset=0;
    double LLx = mLowerLeftX;
    double LLy = mLowerLeftY;
    double URx = mUpperRightX;
    double URy = mUpperRightY;
    // Check each possible area of grid
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
    return levelset;
}

}// Namespace closure
#endif /* INITIALISATIONMODULERECTANGLE_HPP */
