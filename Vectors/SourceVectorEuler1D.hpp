// SourceVector.hpp
//
// Uses MathVector to define a source vector for use in a finite volume scheme.
// Relevant for solving the hydrodynamical semiconductor equations.
//
// Author: Liam Pattinson
// Date: Feb 24 2016

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

#ifndef EULERSOURCEVECTOR_HPP
#define EULERSOURCEVECTOR_HPP

#include "MathVector.hpp"
#include "StateVector.hpp"

namespace Euler{

class SourceVector : public MathVector<3>{
    // x[0] = rho * u
    // x[1] = rho * u^2 + p
    // x[2] = u * (E + p)

public:
    using MathVector<3>::operator=;

    SourceVector() : MathVector() {}
    SourceVector( const double G, const StateVector& State);

    void set( const double G, const StateVector& State);
};

SourceVector::SourceVector( const double G, const StateVector& State){
    set(G,State);
}

void SourceVector::set( const double G, const StateVector& State){
    mX[0] = G;
    mX[1] = State.u()*G;
    mX[2] = (State[2]+State.p())*G/State[0];
    return;
}

}// Namespace closure
#endif /* EULERSOURCEVECTOR_HPP */
