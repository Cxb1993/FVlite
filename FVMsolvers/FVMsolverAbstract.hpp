// FVMsolverAbstract.hpp
//
// Abstract finite volume solver.
// FVM stands for 'Finite Volume Method'

#ifndef FVMABSTRACT_HPP
#define FVMABSTRACT_HPP

#include <string>
#include <iostream>
#include <cstdlib>
#include <libconfig.h++>

#include "Grid/Grid.hpp"
#include "FluxSolvers/FluxSolvers.hpp"
#include "Sources/Sources.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class FVMsolver{

protected:

    Grid*       pGrid;
    FluxSolver* pFlux;
    Source*     pSource;

public:

    FVMsolver(){}
    virtual ~FVMsolver();

    void init( Grid* pGrid, FluxSolver* pFlux, Source* pSource);
    void init( Grid* pGrid, Setting& cfg);
    void source_init( Source* pSource){ (*this).pSource=pSource;}
    virtual void exec( char dim, double t, double dt)=0;
};

// Declare object factory

ObjectFactory<FVMsolver> FVMsolverFactory;

// Function definitions

void FVMsolver::init( Grid* pGrid, FluxSolver* pFlux, Source* pSource){
    (*this).pGrid = pGrid;
    (*this).pFlux = pFlux;
    (*this).pSource =pSource;
    return;
}

void FVMsolver::init( Grid* pGrid, Setting& cfg){
    (*this).pGrid = pGrid;
    string fluxType = cfg.lookup("scheme");
    pFlux = FluxSolverFactory.create(fluxType);
    pFlux->init(pGrid,pSource,cfg);
    return;
}

FVMsolver::~FVMsolver(){
    delete pFlux;
}


}// Namespace closure
#endif /* FVMABSTRACT_HPP */
