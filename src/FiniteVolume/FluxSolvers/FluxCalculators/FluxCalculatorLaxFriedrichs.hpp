// FluxCalculatorLaxFriedrichsBase.hpp
//
// Finite volume flux solver.
// Applies Lax-Friedrichs method.

#ifndef FLUXLF_HPP
#define FLUXLF_HPP

#include "FluxCalculatorAbstract.hpp"

namespace FVlite{

class FluxCalculatorLaxFriedrichs : public FluxCalculator{
public:
    virtual FluxVector exec( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR, const Material& ML, const Material& MR);  
};

REGISTER(FluxCalculator,LaxFriedrichs)

FluxVector FluxCalculatorLaxFriedrichs::exec( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR, const Material& ML, const Material& MR){
    FluxVector result;
    result = BasicFluxes::LaxFriedrichs(ds,dt,dim,UL,UR,ML,MR);
    return result; 
}

}// Namespace closure
#endif /* FLUXLF_HPP */
