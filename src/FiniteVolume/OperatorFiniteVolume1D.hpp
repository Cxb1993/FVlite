// OperatorFiniteVolume1D.hpp
//
// Abstract finite volume solver. Solves 1D case. Can be used in
// higher-dimensional schemes using dimensional splitting.

#ifndef OPERATORFINITEVOLUME1D_HPP
#define OPERATORFINITEVOLUME1D_HPP

#include <string>
#include <iostream>
#include <cstdlib>
#include <libconfig.h++>

#include "Operators/Operator.hpp"
#include "FluxSolvers/OperatorFluxSolver.hpp"
#include "ExplicitUpdaters/OperatorExplicitUpdaters.hpp"
#include "CutCells/OperatorCutCellFluxCorrector.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class OperatorFiniteVolume1D : public CompositeOperator<Operator> {
public:
    virtual void init( Setting& cfg);
};

// Register with factory

REGISTER( Operator, FiniteVolume1D)

// Function definitions

void OperatorFiniteVolume1D::init( Setting& cfg){
    // Build a new flux solver
    OperatorFluxSolver* pFlux = new OperatorFluxSolver;
    // Try to extract "FluxSolver" setting. If not present,
    // print error and exit. Then, set up pFlux.
    try{
        pFlux->init(cfg);
    } catch ( const std::exception& e ){
        std::cerr << e.what() << std::endl;
        std::cerr << "Failed to build FluxSolver" << std::endl;
        exit(EXIT_FAILURE);
    }
    // Build a new explicit updater
    OperatorExplicitUpdater* pUpdate;
    string updateStr;
    try{
        Setting& updateCfg = cfg.lookup("ExplicitUpdater");
        updateStr = updateCfg.lookup("type").c_str();
        pUpdate = static_cast<OperatorExplicitUpdater*>(OperatorFactory.create("ExplicitUpdater"+updateStr));
    } catch ( const std::exception& e){
        std::cerr << e.what() << std::endl;
        std::cerr << "Failed to build ExplicitUpdater" << std::endl;
        exit(EXIT_FAILURE);
    }
    pUpdate->init( cfg); 
    // Are we using CutCells?
    if ( updateStr == "CutCells" ){
        OperatorCutCellFluxCorrector* pCutCells = new OperatorCutCellFluxCorrector( pFlux);
        pCutCells->init(cfg);
        pFlux = pCutCells;
    }
    // Finalise
    add_element( pFlux);
    add_element( pUpdate);
}

}// Namespace closure
#endif /* SOLVERFINITEVOLUME1D_HPP */
