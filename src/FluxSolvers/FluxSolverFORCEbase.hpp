// FluxSolverFORCE.hpp
//
// Finite volume flux solver.
// Applies FORCE method.

#ifndef FLUXFORCE_HPP
#define FLUXFORCE_HPP

#include"FluxSolverAbstract.hpp"

namespace FVlite{

class FluxSolverFORCEbase : public virtual FluxSolver{
public:
    virtual FluxVector getIntercellFlux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR);  
};

FluxVector FluxSolverFORCEbase::getIntercellFlux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR){
    FluxVector result;
    result = 0.5*(BasicFluxes::LaxFriedrichs(ds,dt,dim,UL,UR) + BasicFluxes::Richtmyer(ds,dt,dim,UL,UR));
    return result; 
}

}// Namespace closure
#endif /* FLUXFORCE_HPP */
