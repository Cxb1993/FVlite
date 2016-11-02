// BoundaryModuleSinusoidalBase.hpp
//
// Abstract boundary module.
// Base class for sinusoidal driven boundaries

#ifndef BOUNDARYMODULESINUSOIDALBASE_HPP
#define BOUNDARYMODULESINUSOIDALBASE_HPP

#ifdef MAXWELL

#include <string>
#include <libconfig.h++>

#include "Boundaries/BoundaryModuleDrivenBase.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class BoundaryModuleSinusoidalBase : public BoundaryModuleDrivenBase{
protected:
    double mFreq;
    double mAmp;
public:
    virtual void init( Setting& cfg);
};


void BoundaryModuleSinusoidalBase::init( Setting& cfg){
    BoundaryModuleDrivenBase::init(cfg);
    Setting& params = cfg.lookup("params");
    mFreq = params.lookup("frequency");
    mAmp  = params.lookup("amplitude");
    return;
}


}// Namespace closure
#endif /* MAXWELL */
#endif /* BOUNDARYMODULESINUSOIDALBASE_HPP */
