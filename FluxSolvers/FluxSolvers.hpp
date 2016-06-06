// FluxSolvers.hpp
//
// Include file for finite volume flux solvers.
// Also declares individual sovler types. Solvers are constructed using
// multiple inheritance techniques, so class declarations are very brief.

#ifndef FLUXSOLVERS_HPP
#define FLUXSOLVERS_HPP

#include "FluxSolverAbstract.hpp"
#include "FluxSolverStdBase.hpp"
#include "FluxSolverMUSCLbase.hpp"


#include "FluxSolverRichtmyerBase.hpp"

namespace FVlite{

    class FluxSolverRichtmyer : public FluxSolverStdBase, public FluxSolverRichtmyerBase {};
    REGISTER(FluxSolver,Richtmyer)
}


#include "FluxSolverFORCEbase.hpp"

namespace FVlite{

    class FluxSolverFORCE : public FluxSolverStdBase, public FluxSolverFORCEbase {};
    REGISTER(FluxSolver,FORCE)

    class FluxSolverSLIC : public FluxSolverMUSCLbase, public FluxSolverFORCEbase {};
    REGISTER(FluxSolver,SLIC)
}


#ifdef EULER
#include "FluxSolverHLLCbase.hpp"

namespace FVlite{

    class FluxSolverHLLCfirst : public FluxSolverStdBase, public FluxSolverHLLCbase {};
    REGISTER(FluxSolver,HLLCfirst)

    class FluxSolverHLLC : public FluxSolverMUSCLbase, public FluxSolverHLLCbase{};
    REGISTER(FluxSolver,HLLC)
}

#endif

#endif /* FLUXSOLVERS_HPP */
