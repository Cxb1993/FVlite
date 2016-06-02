// StateVector.hpp
//
// Uses MathVector to define a state vector for use in a finite volume scheme.
// Relevant for solving the Euler eqautions.
//
// Author: Liam Pattinson
// Date: Feb 16 2016

/* Notes on the Euler equations:
 *
 * U_t + F(U)_x = 0
 *     
 * U = [ rho, rho*u, E ]
 * F = [ rho*u, rho*u^2+p, u(E+p) ]
 *
 * U defines the vector of conserved quantities
 *
 * The primitive (physical) variables are:
 *
 * P = [ rho, u, p]
 */

#ifndef EULERSTATEVECTOR_HPP
#define EULERSTATEVECTOR_HPP

#define gam  1.4

#include <cmath>
#include "MathVector.hpp"

namespace Euler{

class StateVector : public MathVector<3>{
    // x[0] = rho
    // x[1] = rho * u
    // x[2] = E

public:
    using MathVector<3>::operator=;

    StateVector() : MathVector() {}
    StateVector( double rho, double u, double p);

    // Set from primitives
    void set( double rho, double u, double p);

    // Primitive variables
    double rho() const;
    double u() const;
    double p() const;

    // Specific internal energy
    double e() const;

    // Approximation of local wave speed
    double a() const;

    // Other
    static unsigned int size(){ return 3;}
};

StateVector::StateVector( double rho, double u, double p){
    set(rho,u,p);
    return;
}

void StateVector::set( double rho, double u, double p){
    x[0] = rho;
    x[1] = rho*u;
    x[2] = 0.5*u*u*rho + p/(gam-1.);
    return;
}

double StateVector::rho() const{
    return x[0];
}

double StateVector::u() const{
    return x[1]/x[0];
}

double StateVector::p() const{
    return e() * (gam-1.) * rho();
}

double StateVector::e() const{
    return x[2]/x[0] - 0.5*u()*u();
}

double StateVector::a() const{
    return sqrt(gam*p()/x[0]);
}

} // Namespace closure
#endif /* EULERSTATEVECTOR_HPP */
