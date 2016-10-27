// SolverFiniteVolume1D.hpp
//
// Abstract finite volume solver. Solves 1D case. Can be used in
// higher-dimensional schemes using dimensional splitting.

#ifndef SOLVERFINITEVOLUME1D_HPP
#define SOLVERFINITEVOLUME1D_HPP

#include <string>
#include <iostream>
#include <cstdlib>
#include <libconfig.h++>

#include "Solvers/SolverDecorator.hpp"
#include "FluxSolvers/FluxSolver.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class SolverFiniteVolume1D : public SolverDecorator {
protected:
    char m_dim;         // sweep direction for dimensional splitting
    double m_dt_ratio;  // the fraction of dt to advance each time step
    FluxSolver* mpFlux;
public:

    SolverFiniteVolume1D(){}
    virtual ~SolverFiniteVolume1D();

    virtual void init( Grid* pGrid, Timer* pTimer, Setting& cfg, Solver* pWrapped);
    virtual void exec();
};

// Register with factory

REGISTER( Solver, FiniteVolume1D)

// Function definitions

void SolverFiniteVolume1D::init( Grid* pGrid, Timer* pTimer, Setting& cfg, Solver* pWrapped){
    SolverDecorator::init(pGrid,pTimer,cfg,pWrapped);
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
    return;
}

void SolverFiniteVolume1D::exec(){
    SolverDecorator::exec();
    int startX = mpGrid->startX();
    int startY = mpGrid->startY();
    int endX = mpGrid->endX();
    int endY = mpGrid->endY();
    double dt = mpTimer->dt() * m_dt_ratio;
    double ds;
    double levelset;
    StateVector State;
    switch(m_dim){
        case 'x' :
            ds = mpGrid->dx();
            // Solve flux
            mpFlux->exec(m_dim,dt);
            // Explicit update formula -- Euler method
            for( int jj=startY; jj<endY; jj++){
                for( int ii=startX; ii<endX; ii++){
                    levelset = mpGrid->levelset(ii,jj);
                    if( levelset > 0) continue;
                    mpGrid->state(ii,jj) = mpGrid->state(ii,jj) + (mpGrid->flux(ii-1,jj)-mpGrid->flux(ii,jj)) * dt/ds;
                }
            }
            break;
        case 'y' :
            ds = mpGrid->dy();
            // Solve flux
            mpFlux->exec(m_dim,dt);
            // Explicit update formula -- Euler method
            for( int jj=startY; jj<endY; jj++){
                for( int ii=startX; ii<endX; ii++){
                    levelset = mpGrid->levelset(ii,jj);
                    if( levelset > 0) continue;
                    mpGrid->state(ii,jj) = mpGrid->state(ii,jj) + (mpGrid->flux(ii,jj-1)-mpGrid->flux(ii,jj)) * dt/ds;
                }
            }
            break;
        case 'z' :
            std::cerr << "Error, z direction not implemented" << std::endl;
            exit(EXIT_FAILURE);
            break;
        default:
            std::cerr << "Error, invalid direction specifier" << std::endl;
            exit(EXIT_FAILURE);
    }
    return;
}


SolverFiniteVolume1D::~SolverFiniteVolume1D(){
    delete mpFlux;
}


}// Namespace closure
#endif /* SOLVERFINITEVOLUME1D_HPP */
