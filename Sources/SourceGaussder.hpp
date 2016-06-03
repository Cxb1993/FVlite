// SourceGaussder.hpp
//
// Generates a Gaussian derivative StateVector in either TE or TM polarisation. Travels in +x direction.
//
// TODO: Generalise sources!

#ifndef SOURCEGAUSSDER_HPP
#define SOURCEGAUSSDER_HPP

#include <iostream>
#include <cmath>

#include "SourceAbstract.hpp"
#include "Vectors/StateVector.hpp"

namespace FVlite{

// Declare Gaussian function
double ScalarGaussder( double, double, double);

// Begin class
class SourceGaussder : public Source{

protected:

    double mSigma;
    double mMean;
    bool   mTE;

public:

    SourceGaussder( double sigma, double mean, bool TE);
    virtual ~SourceGaussder();

    virtual StateVector exec( double t);
};

SourceGaussder::SourceGaussder( double sigma, double mean, bool TE) : mSigma(sigma), mMean(mean), mTE(TE) {}

SourceGaussder::~SourceGaussder(){}

StateVector SourceGaussder::exec( double t){
    StateVector State;
    if(mTE){
        // Transverse electric mode
        double Hz = ScalarGaussder( t, mMean, mSigma);
        double Ey = c_eta0 * Hz;
        State.set(0,Ey,0,0,0,Hz);
    } else {
        // Transverse magnetic mode
        double Hy = ScalarGaussder( t, mMean, mSigma);
        double Ez = c_eta0 * Hy;
        State.set(0,0,Ez,0,Hy,0);
    }
    return State;
}

double ScalarGaussder( double t, double mean, double sigma){
    double result = ((mean-t)/(sigma)) * exp(-(t-mean)*(t-mean)/(2*sigma*sigma));
    return result;
}

}// Namespace closure
#endif /* SOURCEGAUSSDER_HPP */
