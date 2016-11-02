// BoundaryModuleCircle.hpp
//
// Initialises with a circular pattern.

#ifndef INITIALISATIONMODULECIRCLE_HPP
#define INITIALISATIONMODULECIRCLE_HPP

#include <cmath>
#include <libconfig.h++>

#include "InitialisationModuleAbstract.hpp"

using libconfig::Setting;

namespace FVlite{

class InitialisationModuleCircle : public InitialisationModule{

protected:

    double mCenterX;
    double mCenterY;
    double mRadius;

public:

    InitialisationModuleCircle(){}
    virtual void init( Setting& cfg);
    virtual double exec( double x, double y);

};

REGISTER(InitialisationModule,Circle)


void InitialisationModuleCircle::init( Setting& cfg){
    mCenterX = cfg.lookup("center.x");
    mCenterY = cfg.lookup("center.y");
    mRadius  = cfg.lookup("radius");
    return;
}


double InitialisationModuleCircle::exec( double x, double y){
    double distance = sqrt((x-mCenterX)*(x-mCenterX)+(y-mCenterY)*(y-mCenterY));
    return mRadius - distance;
}

}// Namespace closure
#endif /* INITIALISATIONMODULECIRCLE_HPP */
