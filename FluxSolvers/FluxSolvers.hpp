// FluxSolvers.hpp
//
// Include file for finite volume flux solvers.

#ifndef FLUXSOLVERS_HPP
#define FLUXSOLVERS_HPP

#include "FluxSolverAbstract.hpp"

#include "FluxSolverRichtmyer.hpp"
#include "FluxSolverFORCE.hpp"

#include "FluxSolverMUSCLbase.hpp"
#include "FluxSolverSLIC.hpp"
#include "FluxSolverHLLC.hpp"
#include "FluxSolverHLLCfirstorder.hpp"


#include "Limiters/Limiters.hpp"

namespace FVTD{

enum FLUX_TYPE{
    FORCE,
    RICHT,
    SLIC,
    HLLC,
    HLLCFIRST
};

}// Namespace closure
#endif /* FLUXSOLVERS_HPP */
