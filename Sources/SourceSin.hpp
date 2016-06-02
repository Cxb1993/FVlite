// SourceSin.hpp
//
// Generates a sinusoidal StateVector in either TE or TM polarisation. Travels in +x direction.

#ifndef SOURCESIN_HPP
#define SOURCESIN_HPP

#include <iostream>
#include <cmath>

#include "constants.hpp"
#include "SourceAbstract.hpp"
#include "Vectors/StateVector.hpp"

namespace FVTD{

// Declare sinusoid function
double ScalarSin( double, double);

// Begin class
class SourceSin : public Source{

protected:

    double mOmega;
    bool   mTE;

public:

    SourceSin( double omega, bool TE);
    virtual ~SourceSin();

    virtual StateVector exec( double t);
};

SourceSin::SourceSin( double omega, bool TE) : mOmega(omega), mTE(TE) {}

SourceSin::~SourceSin(){}

StateVector SourceSin::exec( double t){
    StateVector State;
    if(mTE){
        // Transverse electric mode
        double Hz = ScalarSin( t, mOmega);
        double Ey = c_eta0 * Hz;
        State.set(0,Ey,0,0,0,Hz);
    } else {
        // Transverse magnetic mode
        double Hy = ScalarSin( t, mOmega);
        double Ez = c_eta0 * Hy;
        State.set(0,0,Ez,0,Hy,0);
    }
    return State;
}

double ScalarSin( double t, double omega){
    double result = sin( omega * t);
    return result;
}

}// Namespace closure
#endif /* SOURCESIN_HPP */
