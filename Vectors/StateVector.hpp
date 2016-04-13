// StateVector.hpp
//
// Uses MathVector to define a state vector for use in a finite volume scheme.

#ifndef STATEVECTOR_HPP
#define STATEVECTOR_HPP

#include <cmath>
#include "MathVector.hpp"

namespace FVTD{

#define SIZE 6

class StateVector : public MathVector<SIZE>{

public:
    using MathVector<SIZE>::operator=;

    StateVector() : MathVector() {}
    StateVector( double Ex, double Ey, double Ez, double Hx, double Hy, double Hz);

    inline double Ex() const;
    inline double Ey() const;
    inline double Ez() const;
    inline double Hx() const;
    inline double Hy() const;
    inline double Hz() const;

    inline void set( double Ex, double Ey, double Ez, double Hx, double Hy, double Hz);

    // Utility
    static unsigned int size(){ return SIZE;}
};

StateVector::StateVector( double Ex, double Ey, double Ez, double Hx, double Hy, double Hz){
    set(Ex,Ey,Ez,Hx,Hy,Hz);
    return;
}

double StateVector::Ex() const{
    return mX[0];
}

double StateVector::Ey() const{
    return mX[1];
}

double StateVector::Ez() const{
    return mX[2];
}

double StateVector::Hx() const{
    return mX[3];
}

double StateVector::Hy() const{
    return mX[4];
}

double StateVector::Hz() const{
    return mX[5];
}

void StateVector::set( double Ex, double Ey, double Ez, double Hx, double Hy, double Hz){
    mX[0] = Ex;
    mX[1] = Ey;
    mX[2] = Ez;
    mX[3] = Hx;
    mX[4] = Hy;
    mX[5] = Hz;
    return;
}

} // Namespace closure
#endif /* STATEVECTOR_HPP */
