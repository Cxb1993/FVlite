// BoundaryFunctions.hpp
//
// Contains functions used to generate boundary conditions
// Functions stored here to allow calls from outside BoundaryModule/Manager

#ifndef BOUNDARYFUNCTIONS_HPP
#define BOUNDARYFUNCTIONS_HPP

#include <cstdlib>
#include <iostream>

#include "Vectors/StateVector.hpp"

namespace FVlite{
namespace Boundary{

StateVector Reflective( const StateVector& Ref, const char dim);
StateVector Transmissive( const StateVector& Ref);

// Definitions

StateVector Reflective( const StateVector& Reference, const char dim){
    StateVector Boundary;
#ifdef MAXWELL
    Boundary = Reference;
    switch(dim){
        case 'x':
            Boundary[1] = 0;
            Boundary[2] = 0;
            break;
        case 'y':
            Boundary[0] = 0;
            Boundary[2] = 0;
            break;
        case 'z':
            break;
        default:
            std::cerr << "INCORRECT FLUX DIRECTION, BOUNDARY FUNCTION" << std::endl;
    }
#endif
#ifdef EULER
    Boundary = Reference;
    switch(dim){
        case 'x':
            Boundary[1] = -1.*Reference[1];
            break;
        case 'y':
            Boundary[2] = -1.*Reference[2];
            break;
        case 'z':
            break;
        default:
            std::cerr << "INCORRECT FLUX DIRECTION, BOUNDARY FUNCTION" << std::endl;
    }
#endif
    return Boundary;
}

StateVector Transmissive( const StateVector& Ref){
    return Ref;
}

}}
#endif
