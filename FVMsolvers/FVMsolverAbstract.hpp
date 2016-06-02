// FVMsolverAbstract.hpp
//
// Abstract finite volume solver.

#ifndef FVMABSTRACT_HPP
#define FVMABSTRACT_HPP

#include <iostream>
#include <cstdlib>

#include "Vectors/Grid.hpp"
#include "FluxSolvers/FluxSolvers.hpp"
#include "Sources/Sources.hpp"

namespace FVTD{

class FVMsolver{

protected:

    Grid*       pGrid;
    FluxSolver* pFlux;
    Source*     pSource;

public:

    FVMsolver( Grid* pGrid, FluxSolver* pFlux, Source* pSource);
    virtual ~FVMsolver();

    virtual void exec( char dim, double t, double dt)=0;
};

FVMsolver::FVMsolver( Grid* pGrid, FluxSolver* pFlux, Source* pSource) : pGrid(pGrid), pFlux(pFlux), pSource(pSource){}

FVMsolver::~FVMsolver(){
    delete pFlux;
}


}// Namespace closure
#endif /* FVMABSTRACT_HPP */
