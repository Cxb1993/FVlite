// FluxSolverLaxFriedrichsBase.hpp
//
// Finite volume flux solver.
// Applies Lax-Friedrichs method.

#ifndef FLUXLF_HPP
#define FLUXLF_HPP

#include"FluxSolverAbstract.hpp"

namespace FVlite{

class FluxSolverLaxFriedrichsBase : public virtual FluxSolver{
public:
    virtual FluxVector getIntercellFlux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR);  
};

FluxVector FluxSolverLaxFriedrichsBase::getIntercellFlux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR){
    FluxVector result;
    result = BasicFluxes::LaxFriedrichs(ds,dt,dim,UL,UR);
    return result; 
}

}// Namespace closure
#endif /* FLUXLF_HPP */
