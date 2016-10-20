// FluxCalculatorFORCE.hpp
//
// Finite volume flux solver.
// Applies FORCE method.

#ifndef FLUXFORCE_HPP
#define FLUXFORCE_HPP

#include"FluxCalculatorAbstract.hpp"

namespace FVlite{

class FluxCalculatorFORCE : public FluxCalculator{
public:
    virtual FluxVector exec(
        double ds, double dt, char dim,
        const StateVector& UL, const StateVector& UR
    );  
};

REGISTER(FluxCalculator,FORCE)

FluxVector FluxCalculatorFORCE::exec( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR){
    FluxVector result;
    result = 0.5*(BasicFluxes::LaxFriedrichs(ds,dt,dim,UL,UR) + BasicFluxes::Richtmyer(ds,dt,dim,UL,UR));
    return result; 
}

}// Namespace closure
#endif /* FLUXFORCE_HPP */
