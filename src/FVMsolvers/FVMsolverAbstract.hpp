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
#include "FluxSolvers/FluxSolver.hpp"
#include "Sources/Sources.hpp"
#include "CutCellManagers/CutCellManagers.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class FVMsolver{

protected:

    Grid*       pGrid;
    CutCellManager* pCutCell;

    FluxSolver* pFlux;

public:

    FVMsolver(){}
    virtual ~FVMsolver();

    void init( Grid* pGrid, Setting& cfg);
    virtual void exec( char dim, double dt) = 0;
    virtual void newTimeStep();
};

// Declare object factory

ObjectFactory<FVMsolver> FVMsolverFactory;

// Function definitions

void FVMsolver::init( Grid* pGrid, Setting& cfg){
    (*this).pGrid = pGrid;

    pFlux = new FluxSolver;
    // Try to extract "FluxSolver" setting. If not present,
    // print error and exit. Then, set up pFlux.
    try{
        Setting& FluxCfg = cfg.lookup("FluxSolver");
        pFlux->init(pGrid,FluxCfg);
    } catch ( const std::exception& e ){
        std::cerr << "test" << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    string fvmType, cutCellType;
    fvmType = cfg.lookup("type").c_str();
    if( fvmType == "Std") cutCellType = "None";
    if( fvmType == "CutCell") cutCellType = "Std";
    pCutCell = CutCellManagerFactory.create(cutCellType);
    pCutCell->init(pGrid,pFlux);
    return;
}

FVMsolver::~FVMsolver(){
    delete pFlux;
    delete pCutCell;
}

void FVMsolver::newTimeStep(){
    pCutCell->newTimeStepSetup();
    return;
}

}// Namespace closure
#endif /* FVMABSTRACT_HPP */
