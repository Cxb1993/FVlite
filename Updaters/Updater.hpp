// Updater.hpp
//
// Abstract finite volume grid updater.
//
// Author: Liam Pattinson
// Date: Apr 7 2016

#ifndef UPDATER_HPP
#define UPDATER_HPP

#include <iostream> // for testing

#include "Grid/Grid.hpp"
#include "Timer/Timer.hpp"
#include "Boundaries/BoundaryManager.hpp"
#include "FVMsolvers/FVMsolvers.hpp"

namespace FVlite{
class Updater{

protected:

    Grid*             pGrid;
    Timer*            pTimer;
    FVMsolver*        pFVM;
    BoundaryManager*  pBmanager;
    CutCellManager*   pCutCell;

public:

    Updater( Grid* pGrid, Timer* pTimer, FVMsolver* pFVM, BoundaryManager* pBmanager);
    virtual ~Updater();

    void exec();
    void checkGrid();
};

Updater::Updater( Grid* pGrid, Timer* pTimer, FVMsolver* pFVM, BoundaryManager* pBmanager)
        : pGrid(pGrid), pTimer(pTimer), pFVM(pFVM), pBmanager(pBmanager){
}

Updater::~Updater(){
    delete pFVM;
    delete pBmanager;
}

void Updater::exec(){

    // Calibrate time step using largest speed on the grid
    pTimer->calibrate_timestep();
    double t  = pTimer->t();
    double dt = pTimer->dt();

    // Tell FVMsolver that a new time step is occurring
    pFVM->newTimeStep();

    // Print current time to screen
    std::cout << "\rTime: " << t
#ifdef DEBUG 
        << std::endl;
#else
        << std::flush;
#endif

    // Makes use of Strang directional splitting. 
    char dim;
    (void) t;

    dim = 'x';
    pBmanager->exec(dim,t,dt);
    pFVM->exec(dim,dt); 

#ifdef DEBUG
    checkGrid();
#endif

    dim = 'y';
    pBmanager->exec(dim,t,dt);
    pFVM->exec(dim,dt);

#ifdef DEBUG
    checkGrid();
#endif

//    dim = 'x';
//    pBmanager->exec(dim,t,dt);
//    pFVM->exec(dim,0.5*dt);
//
//#ifdef DEBUG
//    checkGrid();
//#endif

    // Increment time
    pTimer->advance();

    return;
}

void Updater::checkGrid(){
    int startX = pGrid->startX();
    int startY = pGrid->startY();
    int endX = pGrid->endX();
    int endY = pGrid->endY();
    StateVector State;
    FluxVector Flux;
    bool abort = false;
    for( int jj=startY; jj<endY; jj++){
        for( int ii=startX; ii<endX; ii++){
            State = pGrid->state(ii,jj);
            Flux  = pGrid->flux(ii,jj);
            if( State.isnan() ){
                std::cerr << "State("<<ii<<","<<jj<<") contains NaN" << std::endl;
                abort = true;
            }
            if( Flux.isnan() ){
                std::cerr << "Flux("<<ii<<","<<jj<<") contains NaN" << std::endl;
                abort = true;
            }
        }
    }
    if( abort ){
        std::cerr << "NaNs found in grid. Aborting." << std::endl;
        exit(EXIT_FAILURE);
    }
    return;
}

}// Namespace closure
#endif /* UPDATER_HPP */
