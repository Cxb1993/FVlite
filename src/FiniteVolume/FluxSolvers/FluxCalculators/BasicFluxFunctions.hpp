// BasicFluxFunctions.hpp
//
// Contains basic functions that return intercell fluxes
// Used to build more complicated schemes

#ifndef BASICFLUXES_HPP
#define BASICFLUXES_HPP

#include "Vectors/StateVector.hpp"
#include "Vectors/FluxVector.hpp"
#include "Materials/Material.hpp"

namespace FVlite{
namespace BasicFluxes{

FluxVector LaxFriedrichs( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR, const Material& ML, const Material& MR){
    FluxVector FL, FR, result;
    FL.set(UL,ML,dim);
    FR.set(UR,MR,dim);
    result = 0.5*(FL+FR+(ds/dt)*(UL-UR));
    return result;
}

FluxVector Richtmyer( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR, const Material& ML, const Material& MR){
    FluxVector FL, FR, result;
    StateVector Inter;
    FL.set(UL,ML,dim);
    FR.set(UR,MR,dim);
    Inter = 0.5*(UL+UR+(dt/ds)*(FL-FR));
    // Bias left material
    // TODO: Fix this!
    result.set(Inter,ML,dim);
    return result;
}

}// namespace closure
}// namespace closure
#endif /* BASICFLUXES_HPP */
