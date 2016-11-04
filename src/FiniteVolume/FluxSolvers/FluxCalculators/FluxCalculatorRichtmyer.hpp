// FluxCalculatorRichtmyer.hpp
//
// Finite volume flux solver.
// Applies FORCE method.

#ifndef FLUXRICHT_HPP
#define FLUXRICHT_HPP

#include"FluxCalculatorAbstract.hpp"

namespace FVlite{

class FluxCalculatorRichtmyer : public FluxCalculator{
public:
    virtual FluxVector exec( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR);  
};

REGISTER(FluxCalculator,Richtmyer)

FluxVector FluxCalculatorRichtmyer::exec( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR){
    FluxVector result;
    result = BasicFluxes::Richtmyer(ds,dt,dim,UL,UR);
    return result; 
}

}// Namespace closure
#endif /* FLUXRICHT_HPP */
