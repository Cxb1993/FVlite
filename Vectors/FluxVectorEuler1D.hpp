// FluxVector.hpp
//
// Uses MathVector to define a flux vector for use in a finite volume scheme.
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

#ifndef EULERFLUXVECTOR_HPP
#define EULERFLUXVECTOR_HPP

#include "MathVector.hpp"
#include "StateVector.hpp"

namespace Euler{

class FluxVector : public MathVector<3>{
    // x[0] = rho * u
    // x[1] = rho * u^2 + p
    // x[2] = u * (E + p)

public:
    using MathVector<3>::operator=;

    FluxVector() : MathVector() {}
    FluxVector( const StateVector& State);

    void set( const StateVector& State);
};

FluxVector::FluxVector( const StateVector& State){
    set(State);
}

void FluxVector::set( const StateVector& State){
    x[0] = State[1];
    x[1] = State[1]*State.u() + State.p();
    x[2] = State.u()*(State[2]+State.p());
    return;
}

}// Namespace closure
#endif /* EULERFLUXVECTOR_HPP */
