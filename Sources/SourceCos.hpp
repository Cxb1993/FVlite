// SourceCos.hpp
//
// Generates a (co)sinusoidal StateVector in either TE or TM polarisation. Travels in +x direction.

#ifndef SOURCECOS_HPP
#define SOURCECOS_HPP

#include <iostream>
#include <cmath>

#include "constants.hpp"
#include "SourceAbstract.hpp"
#include "Vectors/StateVector.hpp"

namespace FVlite{

// Declare sinusoid function
double ScalarCos( double, double);

// Begin class
class SourceCos : public Source{

protected:

    double mOmega;
    bool   mTE;

public:

    SourceCos( double omega, bool TE);
    virtual ~SourceCos();

    virtual StateVector exec( double t);
};

SourceCos::SourceCos( double omega, bool TE) : mOmega(omega), mTE(TE) {}

SourceCos::~SourceCos(){}

StateVector SourceCos::exec( double t){
    StateVector State;
    if(mTE){
        // Transverse electric mode
        double Hz = ScalarCos( t, mOmega);
        double Ey = c_eta0 * Hz;
        State.set(0,Ey,0,0,0,Hz);
    } else {
        // Transverse magnetic mode
        double Hy = ScalarCos( t, mOmega);
        double Ez = c_eta0 * Hy;
        State.set(0,0,Ez,0,Hy,0);
    }
    return State;
}

double ScalarCos( double t, double omega){
    double result = cos( omega * t);
    return result;
}

}// Namespace closure
#endif /* SOURCECOS_HPP */
