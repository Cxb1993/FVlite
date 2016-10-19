// BoundaryModuleGaussDer.hpp
//
// Implements Gaussian-derivative driven boundary conditions.

#ifndef BOUNDARYMODULEGAUSSDER_HPP
#define BOUNDARYMODULEGAUSSDER_HPP

#ifdef MAXWELL

#include <cmath>
#include"BoundaryModuleGaussianBase.hpp"

namespace FVlite{

class BoundaryModuleGaussDer : public BoundaryModuleGaussianBase{
public:
    virtual double getDrivenTerm( const double t);
};

REGISTER(BoundaryModule,GaussDer)

double BoundaryModuleGaussDer::getDrivenTerm( const double t){
    double numer = (t - mMean)*(t - mMean);
    double denom = 2*mStdDev*mStdDev;
    return ((mMean-t)/mStdDev)*mAmp*exp(-numer/denom);
}

}// Namespace closure
#endif /* MAXWELL */
#endif /* BOUNDARYMODULEGAUSSDER_HPP */
