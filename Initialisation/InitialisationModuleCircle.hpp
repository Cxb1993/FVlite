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

class InitialisationModuleCircle{

protected:

    double mCenterX;
    double mCenterY;
    double mRadius;

public:

    InitialisationModuleCircle(){}
    virtual void init_params( Setting& cfg);
    virtual double getLevelSet( double x, double y);

};

REGISTER(InitialisationModule,Circle)


void InitialisationModuleCircle::init_params( Setting& cfg){
    mCenterX = cfg.lookup("center.x");
    mCenterY = cfg.lookup("center.y");
    mRadius  = cfg.lookup("radius");
    if( !mSolid ){
        Setting& stateCfg = cfg.lookup("state");
        // set values to a MathVector, not a StateVector
        // This stores the primitive values, which are converted
        // to a conserved form using the set(MathVector) function.
        MathVector<StateVector::size()> vector;
        for( int ii=0; ii<StateVector::size(); ii++){
            vector[ii] = stateCfg[ii];
        }
        mState.set(vector);
    }
    return;
}


double InitialisationModuleCircle::getLevelSet( double x, double y){
    double distance = sqrt((x-mCenterX)*(x-mCenterX)+(y-mCenterY)*(y-mCenterY));
    return mRadius - distance;
}

}// Namespace closure
#endif /* INITIALISATIONMODULECIRCLE_HPP */
