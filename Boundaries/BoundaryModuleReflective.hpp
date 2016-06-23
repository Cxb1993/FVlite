// BoundaryModuleReflective.hpp
//
// Implements reflecting boundary conditions.
// When in use with Maxwell's equations, they behave as PEC boundary conditions

#ifndef BOUNDARYMODULEREFLECTIVE_HPP
#define BOUNDARYMODULEREFLECTIVE_HPP

#include <cstdlib>
#include <iostream>

#include"BoundaryModuleAbstract.hpp"

namespace FVlite{

class BoundaryModuleReflective : public BoundaryModule{
public:
    virtual StateVector getBoundary( const StateVector& Reference, const char dim, const double t);
};

REGISTER(BoundaryModule,Reflective)

StateVector BoundaryModuleReflective::getBoundary( const StateVector& Reference, const char dim, const double t){
    (void)t;
    StateVector Boundary;
    Boundary = Boundary::Reflective( Reference, dim);
    return Boundary;
}

}// Namespace closure
#endif /* BOUNDARYMODULEREFLECTIVE_HPP */
