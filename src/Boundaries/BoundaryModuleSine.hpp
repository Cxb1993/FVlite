// BoundaryModuleSine.hpp
//
// Implements sine driven boundary conditions.

#ifndef BOUNDARYMODULESINE_HPP
#define BOUNDARYMODULESINE_HPP

#ifdef MAXWELL

#include <cmath>
#include"BoundaryModuleSinusoidalBase.hpp"

namespace FVlite{

class BoundaryModuleSine : public BoundaryModuleSinusoidalBase{
public:
    virtual double getDrivenTerm( const double t);
};

REGISTER(BoundaryModule,Sine)

double BoundaryModuleSine::getDrivenTerm( const double t){
    return mAmp*sin(mFreq*t);
}

}// Namespace closure
#endif /* MAXWELL */
#endif /* BOUNDARYMODULESINE_HPP */
