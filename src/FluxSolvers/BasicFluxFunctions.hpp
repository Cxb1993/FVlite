// BasicFluxFunctions.hpp
//
// Contains basic functions that return intercell fluxes
// Used to build more complicated schemes

#ifndef BASICFLUXES_HPP
#define BASICFLUXES_HPP

#include "Vectors/StateVector.hpp"
#include "Vectors/FluxVector.hpp"

namespace FVlite{
namespace BasicFluxes{

FluxVector LaxFriedrichs( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR){
    FluxVector FL, FR, result;
    FL.set(UL,dim);
    FR.set(UR,dim);
    result = 0.5*(FL+FR+(ds/dt)*(UL-UR));
    return result;
}

FluxVector Richtmyer( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR){
    FluxVector FL, FR, result;
    StateVector Inter;
    FL.set(UL,dim);
    FR.set(UR,dim);
    Inter = 0.5*(UL+UR+(dt/ds)*(FL-FR));
    result.set(Inter,dim);
    return result;
}

}// namespace closure
}// namespace closure
#endif /* BASICFLUXES_HPP */
