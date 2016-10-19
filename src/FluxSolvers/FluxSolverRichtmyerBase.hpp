// FluxSolverRichtmyer.hpp
//
// Finite volume flux solver.
// Applies FORCE method.

#ifndef FLUXRICHT_HPP
#define FLUXRICHT_HPP

#include"FluxSolverAbstract.hpp"

namespace FVlite{

class FluxSolverRichtmyerBase : public virtual FluxSolver{
public:
    virtual FluxVector getIntercellFlux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR);  
};

FluxVector FluxSolverRichtmyerBase::getIntercellFlux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR){
    FluxVector result;
    result = BasicFluxes::Richtmyer(ds,dt,dim,UL,UR);
    return result; 
}

}// Namespace closure
#endif /* FLUXRICHT_HPP */
