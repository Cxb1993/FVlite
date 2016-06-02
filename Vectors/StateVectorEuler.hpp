// StateVectorEuler.hpp
//
// Uses MathVector to define a state vector for use in a finite volume scheme.
// Implements Euler equations

#ifndef STATEVECTOR_HPP
#define STATEVECTOR_HPP

#include <cmath>
#include "MathVector.hpp"
#include "constants.hpp"

namespace FVTD{

#define SIZE 4

// Current code assumes ideal gas conditions
// TODO generalise this

class StateVector : public MathVector<SIZE>{
// x[0] = rho
// x[1] = rho*ux
// x[2] = rho*uy
// x[3] = E
// Set with primitive variables. Pass in pressure, not energy.

public:
    using MathVector<SIZE>::operator=;

    StateVector() : MathVector() {}
    StateVector( double rho, double ux, double uy, double p);

    // Access conserved variables
    inline double rho() const { return mX[0];}
    inline double rhoUx() const { return mX[1];}
    inline double rhoUy() const { return mX[2];}
    inline double E() const { return mX[3];}
    inline double& rho() { return mX[0];}
    inline double& rhoUx() { return mX[1];}
    inline double& rhoUy() { return mX[2];}
    inline double& E() { return mX[3];}

    // Access primitive variables / other variables
    inline double ux() const { return rhoUx()/rho();} // x velocity
    inline double uy() const { return rhoUy()/rho();} // y velocity
    inline double e() const { return p()/(rho()*(c_gamma_ideal-1.));} // specific internal energy
    inline double a() const { return sqrt(c_gamma_ideal*p()/rho());} // local wave speed (scalar)
    inline double p() const { return (c_gamma_ideal-1.)*(E()-0.5*rho()*(ux()*ux()+uy()*uy()));} // pressure

    inline void set( double rho, double ux, double uy, double p);

    // Utility
    static unsigned int size(){ return SIZE;}
};



StateVector::StateVector( double rho, double ux, double uy, double p){
    set(rho,ux,uy,p);
    return;
}

void StateVector::set( double rho, double ux, double uy, double p){
    mX[0] = rho;
    mX[1] = rho*ux;
    mX[2] = rho*uy;
    mX[3] = 0.5*rho*(ux*ux+uy*uy) + p/(c_gamma_ideal-1.);
    return;
}

// Define zero state

StateVector ZEROSTATE(0,0,0,0);

} // Namespace closure
#endif /* STATEVECTOR_HPP */
