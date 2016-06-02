// FluxVector.hpp
//
// Uses MathVector to define a flux vector for use in a finite volume scheme.

#ifndef FLUXVECTOR_HPP
#define FLUXVECTOR_HPP

#include <iostream>
#include <cstdlib>

#include "MathVector.hpp"
#include "StateVector.hpp"
#include "constants.hpp"
#include "Material.hpp"

// constant SIZE defined in StateVector.hpp

namespace FVTD{

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
    switch(dim){
        case 'x' :
            mX[0] = 0;
            mX[1] = State.Hz()/c_eps0;
            mX[2] = -State.Hy()/c_eps0;
            mX[3] = 0;
            mX[4] = -State.Ez()/c_mu0;
            mX[5] = State.Ey()/c_mu0;
            break;
        case 'y' :
            mX[0] = -State.Hz()/c_eps0;
            mX[1] = 0;
            mX[2] = State.Hx()/c_eps0;
            mX[3] = State.Ez()/c_mu0;
            mX[4] = 0;
            mX[5] = -State.Ex()/c_mu0;
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
