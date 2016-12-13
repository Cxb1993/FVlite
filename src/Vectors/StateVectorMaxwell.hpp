// StateVector.hpp
//
// Uses MathVector to define a state vector for use in a finite volume scheme.

#ifndef STATEVECTOR_HPP
#define STATEVECTOR_HPP

#include <cmath>
#include "MathVector.hpp"

namespace FVlite{

#define SIZE 6

class StateVector : public MathVector<SIZE>{

public:
    using MathVector<SIZE>::operator=;

    StateVector() : MathVector() {}
    StateVector( double Ex, double Ey, double Ez, double Hx, double Hy, double Hz);

    double Ex() const { return mX[0];}
    double Ey() const { return mX[1];}
    double Ez() const { return mX[2];}
    double Hx() const { return mX[3];}
    double Hy() const { return mX[4];}
    double Hz() const { return mX[5];}
    double& Ex(){ return mX[0];}
    double& Ey(){ return mX[1];}
    double& Ez(){ return mX[2];}
    double& Hx(){ return mX[3];}
    double& Hy(){ return mX[4];}
    double& Hz(){ return mX[5];}

    inline void set( double Ex, double Ey, double Ez, double Hx, double Hy, double Hz);
    inline void set( MathVector<SIZE> fields);

    // Utility
    static unsigned int size(){ return SIZE;}
};

StateVector::StateVector( double Ex, double Ey, double Ez, double Hx, double Hy, double Hz){
    set(Ex,Ey,Ez,Hx,Hy,Hz);
    return;
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

void StateVector::set( MathVector<SIZE> fields){
    set(fields[0],fields[1],fields[2],fields[3],fields[4],fields[5]);
    return;
}

// define zero state
StateVector ZEROSTATE(0,0,0,0,0,0);

} // Namespace closure
#endif /* STATEVECTOR_HPP */
