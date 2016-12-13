// FluxVector.hpp
//
// Uses MathVector to define a flux vector for use in a finite volume scheme.

#ifndef FLUXVECTOR_HPP
#define FLUXVECTOR_HPP

#include <iostream>
#include <cstdlib>

#include "MathVector.hpp"
#include "StateVector.hpp"
#include "MaterialElectromagnetic.hpp"

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
    switch(dim){
        case 'x' :
            mX[0] =  0;
            mX[1] =  state.Hz() * mat.epsilon_reciprocal();
            mX[2] = -state.Hy() * mat.epsilon_reciprocal();
            mX[3] =  0;
            mX[4] = -state.Ez() * mat.mu_reciprocal();
            mX[5] =  state.Ey() * mat.mu_reciprocal();
            break;
        case 'y' :
            mX[0] = -state.Hz() * mat.epsilon_reciprocal();
            mX[1] =  0;
            mX[2] =  state.Hx() * mat.epsilon_reciprocal();
            mX[3] =  state.Ez() * mat.mu_reciprocal();
            mX[4] =  0;
            mX[5] = -state.Ex() * mat.mu_reciprocal();
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
