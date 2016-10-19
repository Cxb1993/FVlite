// FluxVectorEuler.hpp
//
// Uses MathVector to define a flux vector for use in a finite volume scheme.
// Flux vector for Euler equations.

#ifndef FLUXVECTOR_HPP
#define FLUXVECTOR_HPP

#include <iostream>
#include <cstdlib>

#include "MathVector.hpp"
#include "StateVector.hpp"

// constant SIZE defined in StateVector.hpp

namespace FVlite{

class FluxVector : public MathVector<SIZE>{

public:
    using MathVector<SIZE>::operator=;

    FluxVector() : MathVector() {}
    FluxVector( const StateVector& State, const char dim);

    inline void set( const StateVector& State, const char dim);
};

FluxVector::FluxVector( const StateVector& State, const char dim){
    set(State,dim);
}

void FluxVector::set( const StateVector& State, const char dim){
    double ux = State.ux();
    double uy = State.uy();
    switch(dim){
        case 'x' :
            mX[0] = State.rhoUx();
            mX[1] = State.rho()*(ux*ux) + State.p();
            mX[2] = State.rhoUx()*uy;
            mX[3] = ux*(State.E() + State.p());
            break;
        case 'y' :
            mX[0] = State.rhoUy();
            mX[1] = State.rhoUx()*uy;
            mX[2] = State.rho()*(uy*uy) + State.p();
            mX[3] = uy*(State.E() + State.p());
            break;
        case 'z' :
            std::cerr << "Error, z direction not implemented" << std::endl;
            exit(EXIT_FAILURE);
            break;
        default:
            std::cerr << "Error, invalid flux direction" << std::endl;
            exit(EXIT_FAILURE);
    }
    return;
}

}// Namespace closure
#endif /* FLUXVECTOR_HPP */
