// BoundaryModuleBackground.hpp
//
// Initialises background. Not recommended to couple this with the 'solid' or 'outer' settings!

#ifndef INITIALISATIONMODULEBACKGROUND_HPP
#define INITIALISATIONMODULEBACKGROUND_HPP

#include <cmath>
#include <libconfig.h++>

using libconfig::Setting;

namespace FVlite{

class InitialisationModuleBackground{

public:

    InitialisationModuleBackground(){}
    virtual void init_params( Setting& cfg);
    virtual double getLevelSet( double x, double y);

};

REGISTER(InitialisationModule,Background)


void InitialisationModuleBackground::init_params( Setting& cfg){
    Setting& stateCfg = cfg.lookup("state");
    // set values to a MathVector, not a StateVector
    // This stores the primitive values, which are converted
    // to a conserved form using the set(MathVector) function.
    MathVector<StateVector::size()> vector;
    for( int ii=0; ii<StateVector::size(); ii++){
        vector[ii] = stateCfg[ii];
    }
    mState.set(vector);
    return;
}


double InitialisationModuleBackground::getLevelSet( double x, double y){
    (void)x;
    (void)y;
    return -1.;
}

}// Namespace closure
#endif /* INITIALISATIONMODULEBACKGROUND_HPP */
