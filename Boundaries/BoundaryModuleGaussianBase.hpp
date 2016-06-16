// BoundaryModuleGaussianBase.hpp
//
// Abstract boundary module.
// Base class for Gaussian driven boundaries

#ifndef BOUNDARYMODULEGAUSSIANBASE_HPP
#define BOUNDARYMODULEGAUSSIANBASE_HPP

#ifdef MAXWELL

#include <string>
#include <libconfig.h++>

#include "Boundaries/BoundaryModuleDrivenBase.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class BoundaryModuleGaussianBase : public BoundaryModuleDrivenBase{
protected:
    double mAmp;
    double mStdDev;
    double mMean;
public:
    virtual void init( Grid* pGrid, Setting& cfg);
};


void BoundaryModuleGaussianBase::init( Grid* pGrid, Setting& cfg){
    BoundaryModuleDrivenBase::init(pGrid,cfg);
    Setting& params = cfg.lookup("params");
    mAmp = params.lookup("amplitude");
    mStdDev = params.lookup("stddev");
    mMean  = params.lookup("mean");
    return;
}


}// Namespace closure
#endif /* MAXWELL */
#endif /* BOUNDARYMODULEGAUSSIANBASE_HPP */
