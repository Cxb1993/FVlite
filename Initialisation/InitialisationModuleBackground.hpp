// BoundaryModuleBackground.hpp
//
// Initialises background. Not recommended to couple this with the 'solid' or 'outer' settings!

#ifndef INITIALISATIONMODULEBACKGROUND_HPP
#define INITIALISATIONMODULEBACKGROUND_HPP

#include <cmath>
#include <libconfig.h++>

#include "InitialisationModuleAbstract.hpp"

using libconfig::Setting;

namespace FVlite{

class InitialisationModuleBackground : public InitialisationModule{

public:

    InitialisationModuleBackground(){}
    virtual void init_params( Setting& cfg);
    virtual double getLevelSet( double x, double y);

};

REGISTER(InitialisationModule,Background)


void InitialisationModuleBackground::init_params( Setting& cfg){
    (void)cfg;
    return;
}


double InitialisationModuleBackground::getLevelSet( double x, double y){
    (void)x;
    (void)y;
    return 1.;
}

}// Namespace closure
#endif /* INITIALISATIONMODULEBACKGROUND_HPP */
