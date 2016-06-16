// BoundaryModuleGaussian.hpp
//
// Implements Gaussian driven boundary conditions.

#ifndef BOUNDARYMODULEGAUSSIAN_HPP
#define BOUNDARYMODULEGAUSSIAN_HPP

#ifdef MAXWELL

#include <cmath>
#include"BoundaryModuleGaussianBase.hpp"

namespace FVlite{

class BoundaryModuleGaussian : public BoundaryModuleGaussianBase{
public:
    virtual double getDrivenTerm( const double t);
};

REGISTER(BoundaryModule,Gaussian)

double BoundaryModuleGaussian::getDrivenTerm( const double t){
    double numer = (t - mMean)*(t - mMean);
    double denom = 2*mStdDev*mStdDev;
    return mAmp*exp(-numer/denom);
}

}// Namespace closure
#endif /* MAXWELL */
#endif /* BOUNDARYMODULEGAUSSIAN_HPP */
