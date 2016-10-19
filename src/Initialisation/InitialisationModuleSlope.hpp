// BoundaryModuleSlope.hpp
//
// Initialises with a slope pattern.

#ifndef INITIALISATIONMODULESLOPE_HPP
#define INITIALISATIONMODULESLOPE_HPP

#include <cmath>
#include <libconfig.h++>

#include "InitialisationModuleAbstract.hpp"

using libconfig::Setting;

namespace FVlite{

class InitialisationModuleSlope : public InitialisationModule{

protected:

    double mPoint1X;
    double mPoint1Y;
    double mPoint2X;
    double mPoint2Y;

public:

    InitialisationModuleSlope(){}
    virtual void init_params( Setting& cfg);
    virtual double getLevelSet( double x, double y);

};

REGISTER(InitialisationModule,Slope)


void InitialisationModuleSlope::init_params( Setting& cfg){
    mPoint1X = cfg.lookup("point1.x");
    mPoint1Y = cfg.lookup("point1.y");
    mPoint2X = cfg.lookup("point2.x");
    mPoint2Y = cfg.lookup("point2.y");
    return;
}


double InitialisationModuleSlope::getLevelSet( double x, double y){
    // copied code form older source, hence the pointless redefinitions
    double x1=mPoint1X;
    double x2=mPoint2X;
    double y1=mPoint1Y;
    double y2=mPoint2Y;
    double distance = ((y2-y1)*x - (x2-x1)*y + x2*y1 - y2*x1)/sqrt((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1));
    return distance;
}

}// Namespace closure
#endif /* INITIALISATIONMODULESLOPE_HPP */
