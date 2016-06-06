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

public:

    Updater( Grid* pGrid, Timer* pTimer, FVMsolver* pFVM, BoundaryManager* pBmanager);
    virtual ~Updater();

    void exec();
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
    std::cout << "\rTime: " << t << std::flush;
    // Makes use of Strang directional splitting. 
    char dim;

    dim = 'x';
    pBmanager->exec(dim);
    pFVM->exec(dim,t,0.5*dt); 

    dim = 'y';
    pBmanager->exec(dim);
    pFVM->exec(dim,t+0.5*dt,dt);

    dim = 'x';
    pBmanager->exec(dim);
    pFVM->exec(dim,t+0.5*dt,0.5*dt);

    // Increment time
    pTimer->advance();

    return;
}

}// Namespace closure
#endif /* UPDATER_HPP */
