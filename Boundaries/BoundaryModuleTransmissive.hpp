// BoundaryModuleTransmissive.hpp
//
// Implements reflecting boundary conditions.
// Well in use with Maxwell's equations, they behave as PEC boundary conditions

#ifndef BOUNDARYMODULERTRANSMISSIVE_HPP
#define BOUNDARYMODULERTRANSMISSIVE_HPP

#include <cstdlib>
#include <iostream>

#include"BoundaryModuleAbstract.hpp"

namespace FVlite{

class BoundaryModuleTransmissive : public BoundaryModule{
public:
    BoundaryModuleTransmissive(){}
    ~BoundaryModuleTransmissive(){}
    virtual StateVector getBoundary( const StateVector& Reference, const char dim, const double t, const double dt);
};

REGISTER(BoundaryModule,Transmissive)

StateVector BoundaryModuleTransmissive::getBoundary( const StateVector& Reference, const char dim, const double t, const double dt){
    (void)dim;
    (void)t;
    (void)dt;
    StateVector Boundary;
    Boundary = Boundary::Transmissive( Reference);
    return Boundary;
}

}// Namespace closure
#endif /* BOUNDARYMODULETRANSMISSIVE_HPP */
