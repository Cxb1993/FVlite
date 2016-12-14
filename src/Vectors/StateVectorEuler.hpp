// StateVectorEuler.hpp
//
// Uses MathVector to define a state vector for use in a finite volume scheme.
// Implements Euler equations

#ifndef STATEVECTOR_HPP
#define STATEVECTOR_HPP

#include <cmath>
#include "MathVector.hpp"
#include "Vector3.hpp"
#include "constants.hpp"

namespace FVlite{

#define SIZE 4

class StateVector : public MathVector<SIZE>{

public:
    using MathVector<SIZE>::operator=;

    StateVector() : MathVector() {}
    StateVector( double rho, double rhoux, double rhouy, double E);

    // Access conserved variables
    double rho() const { return mX[0];}
    double rhoUx() const { return mX[1];}
    double rhoUy() const { return mX[2];}
    double E() const { return mX[3];}
    double& rho() { return mX[0];}
    double& rhoUx() { return mX[1];}
    double& rhoUy() { return mX[2];}
    double& E() { return mX[3];}

    // Access primitive variables / other variables
    double ux() const { return rhoUx()/rho();} // x velocity
    double uy() const { return rhoUy()/rho();} // y velocity

    void set( double rho, double rhoux, double rhouy, double E);

    // Utility
    static unsigned int size(){ return SIZE;}
};



StateVector::StateVector( double rho, double rhoux, double rhouy, double E){
    set(rho,rhoux,rhouy,E);
    return;
}

void StateVector::set( double rho, double rhoux, double rhouy, double E){
    mX[0] = rho;
    mX[1] = rhoux;
    mX[2] = rhouy;
    mX[3] = E;
    return;
}

// Define zero state

StateVector ZEROSTATE(0,0,0,0);

} // Namespace closure
#endif /* STATEVECTOR_HPP */
