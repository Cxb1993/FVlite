// SourceGaussian.hpp
//
// Generates a Gaussian StateVector in either TE or TM polarisation. Travels in +x direction.
//
// TODO: Generalise sources!

#ifndef SOURCEGAUSSIAN_HPP
#define SOURCEGAUSSIAN_HPP

#include <iostream>
#include <cmath>

#include "SourceAbstract.hpp"
#include "Vectors/StateVector.hpp"

namespace FVlite{

// Declare Gaussian function
double ScalarGaussian( double, double, double);

// Begin class
class SourceGaussian : public Source{

protected:

    double mSigma;
    double mMean;
    bool   mTE;

public:

    SourceGaussian( double sigma, double mean, bool TE);
    virtual ~SourceGaussian();

    virtual StateVector exec( double t);
};

SourceGaussian::SourceGaussian( double sigma, double mean, bool TE) : mSigma(sigma), mMean(mean), mTE(TE) {}

SourceGaussian::~SourceGaussian(){}

StateVector SourceGaussian::exec( double t){
    StateVector State;
    if(mTE){
        // Transverse electric mode
        double Hz = ScalarGaussian( t, mMean, mSigma);
        double Ey = c_eta0 * Hz;
        State.set(0,Ey,0,0,0,Hz);
    } else {
        // Transverse magnetic mode
        double Hy = ScalarGaussian( t, mMean, mSigma);
        double Ez = c_eta0 * Hy;
        State.set(0,0,Ez,0,Hy,0);
    }
    return State;
}

double ScalarGaussian( double t, double mean, double sigma){
    double result = exp(-(t-mean)*(t-mean)/(2*sigma*sigma));
    return result;
}

}// Namespace closure
#endif /* GAUSSIANSOURCEGAUSSIAN_HPP */
