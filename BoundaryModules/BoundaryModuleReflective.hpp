// BoundaryModuleReflective.hpp
//
// Implements reflecting boundary conditions.
// Well in use with Maxwell's equations, they behave as PEC boundary conditions

#ifndef BOUNDARYMODULEREFLECTIVE_HPP
#define BOUNDARYMODULEREFLECTIVE_HPP

#include <cstdlib>
#include <iostream>

#include"BoundaryModuleAbstract.hpp"

namespace FVlite{

class BoundaryModuleReflective : public BoundaryModule{
public:
    BoundaryModuleReflective(){}
    virtual StateVector getBoundary( StateVector& Reference, const char dim);
};

REGISTER(BoundaryModule,Reflective)

StateVector BoundaryModuleReflective::getBoundary( const StateVector& Reference, const char dim){
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

}// Namespace closure
#endif /* BOUNDARYMODULEREFLECTIVE_HPP */
