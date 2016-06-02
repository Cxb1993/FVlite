// Updater.hpp
//
// Abstract finite volume grid updater.
//
// Author: Liam Pattinson
// Date: Apr 7 2016

#ifndef UPDATER_HPP
#define UPDATER_HPP

#include <iostream> // for testing

#include "Vectors/Grid.hpp"
#include "Timer/Timer.hpp"
#include "BoundaryUpdaters/BoundaryUpdaters.hpp"
#include "FVMsolvers/FVMsolvers.hpp"

namespace FVTD{
class Updater{

protected:

    Grid*             pGrid;
    Timer*            pTimer;
    FVMsolver*        pFVM;
    BoundaryUpdater*  pBUpdate;

public:

    Updater( Grid* pGrid, Timer* pTimer, FVMsolver* pFVM, BoundaryUpdater* pBUpdate);
    virtual ~Updater();

    void exec();
};

Updater::Updater( Grid* pGrid, Timer* pTimer, FVMsolver* pFVM, BoundaryUpdater* pBUpdate)
        : pGrid(pGrid), pTimer(pTimer), pFVM(pFVM), pBUpdate(pBUpdate){
}

Updater::~Updater(){
    delete pFVM;
    delete pBUpdate;
}

void Updater::exec(){
    // Must first calibrate time step using largest speed on the grid
    pBUpdate->exec();
    pTimer->calibrate_timestep();
    double t  = pTimer->t();
    double dt = pTimer->dt();
    std::cout << "\rTime: " << t << std::flush;
    // Makes use of Strang directional splitting. 
    pFVM->exec('x',t,0.5*dt); 
    pBUpdate->exec();
    pFVM->exec('y',t+0.5*dt,dt);
    pBUpdate->exec();
    pFVM->exec('x',t+0.5*dt,0.5*dt);
    // Increment time
    pTimer->advance();
    return;
}

}// Namespace closure
#endif /* UPDATER_HPP */
