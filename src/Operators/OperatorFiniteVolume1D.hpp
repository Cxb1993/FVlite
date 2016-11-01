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

#include "Operator.hpp"
#include "FluxSolvers/FluxSolver.hpp"
#include "ExplicitUpdaters/ExplicitUpdaters.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class OperatorFiniteVolume1D : public Operator {
protected:
    char m_dim;         // sweep direction for dimensional splitting
    double m_dt_ratio;  // the fraction of dt to advance each time step
    FluxSolver* mpFlux;
    ExplicitUpdater* mpUpdate;
public:

    OperatorFiniteVolume1D(){}
    virtual ~OperatorFiniteVolume1D();

    virtual void init( Grid* pGrid, Timer* pTimer, Setting& cfg);
    virtual void exec();
};

// Register with factory

REGISTER( Operator, FiniteVolume1D)

// Function definitions

OperatorFiniteVolume1D::~OperatorFiniteVolume1D(){
    delete mpFlux;
    delete mpUpdate;
}

void OperatorFiniteVolume1D::init( Grid* pGrid, Timer* pTimer, Setting& cfg){
    Operator::init(pGrid,pTimer,cfg);
    // Set time step ratio
    try{
        m_dt_ratio = cfg.lookup("dt_ratio");
    } catch ( const std::exception& e){
        m_dt_ratio = 1.0;
    }
    // Set dimensionality ( sweep direction)
    try{
        m_dim = cfg.lookup("dim").c_str()[0];
    } catch ( const std::exception& e){
        m_dim = 'x';
    }
    // Build a new flux solver
    mpFlux = new FluxSolver;
    // Try to extract "FluxSolver" setting. If not present,
    // print error and exit. Then, set up pFlux.
    try{
        Setting& FluxCfg = cfg.lookup("FluxSolver");
        mpFlux->init(mpGrid,FluxCfg);
    } catch ( const std::exception& e ){
        std::cerr << e.what() << std::endl;
        std::cerr << "Failed to build FluxSolver" << std::endl;
        exit(EXIT_FAILURE);
    }
    // Build a new explicit updater
    try{
        Setting& updateCfg = cfg.lookup("ExplicitUpdater");
        string updateStr = updateCfg.lookup("type");
        mpUpdate = ExplicitUpdaterFactory.create(updateStr);
    } catch ( const std::exception& e){
        std::cerr << "WARNING: Desired updater not found" << std::endl;
        std::cerr << "Setting to Euler" << std::endl;
        mpUpdate = ExplicitUpdaterFactory.create("Euler");
    }
    mpUpdate->init( mpGrid, mpTimer, cfg); 
}

void OperatorFiniteVolume1D::exec(){
    double dt = mpTimer->dt() * m_dt_ratio;
    // Solve flux
    mpFlux->exec(m_dim,dt);
    // Explicit Update
    mpUpdate->exec(m_dim,dt);
}

}// Namespace closure
#endif /* SOLVERFINITEVOLUME1D_HPP */
