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
#include "Materials/Material.hpp"

// constant SIZE defined in StateVector.hpp

namespace FVlite{

class FluxVector : public MathVector<SIZE>{

public:
    using MathVector<SIZE>::operator=;

    FluxVector() : MathVector() {}
    FluxVector( const StateVector& state, const Material& mat, const char dim);

    inline void set( const StateVector& state, const Material& mat, const char dim);
};

FluxVector::FluxVector( const StateVector& state, const Material& mat, const char dim){
    set(state,mat,dim);
}

void FluxVector::set( const StateVector& state, const Material& mat, const char dim){
    // TODO:
    // This switch-case could be avoided with templates. Fairly
    // important as this is a very frequently called function.
    // Also may be able to optimise algebraic steps.
    double ux = state.ux();
    double uy = state.uy();
    switch(dim){
        case 'x' :
            mX[0] = state.rhoUx();
            mX[1] = state.rho()*(ux*ux) + mat.pressure(state);
            mX[2] = state.rhoUx()*uy;
            mX[3] = ux*(state.E() + mat.pressure(state));
            break;
        case 'y' :
            mX[0] = state.rhoUy();
            mX[1] = state.rhoUx()*uy;
            mX[2] = state.rho()*(uy*uy) + mat.pressure(state);
            mX[3] = uy*(state.E() + mat.pressure(state));
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
