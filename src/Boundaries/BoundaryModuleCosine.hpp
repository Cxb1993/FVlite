// BoundaryModuleCosine.hpp
//
// Implements cosine driven boundary conditions.

#ifndef BOUNDARYMODULECOSINE_HPP
#define BOUNDARYMODULECOSINE_HPP

#ifdef MAXWELL

#include <cmath>
#include"BoundaryModuleSinusoidalBase.hpp"

namespace FVlite{

class BoundaryModuleCosine : public BoundaryModuleSinusoidalBase{
public:
    virtual double getDrivenTerm( const double t);
};

REGISTER(BoundaryModule,Cosine)

double BoundaryModuleCosine::getDrivenTerm( const double t){
    return mAmp*cos(mFreq*t);
}

}// Namespace closure
#endif /* MAXWELL */
#endif /* BOUNDARYMODULECOSINE_HPP */
