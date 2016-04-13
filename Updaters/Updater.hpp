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
#include "PDEsolvers/PDEsolvers.hpp"
#include "BoundaryUpdaters/BoundaryUpdaters.hpp"

// Temp includes for testing
#include "Sources/point_source.hpp"


namespace FVTD{
class Updater{

protected:

    Grid*             pGrid;
    Timer*            pTimer;
    PDEsolver*        pPDE;
    BoundaryUpdater*  pBUpdate;

    // Testing
    PointSource* pSource;

public:

    Updater( Grid* pGrid, Timer* pTimer, PDEsolver* pPDE, BoundaryUpdater* pBUpdate);
    virtual ~Updater();

    void exec();
};

Updater::Updater( Grid* pGrid, Timer* pTimer, PDEsolver* pPDE, BoundaryUpdater* pBUpdate)
        : pGrid(pGrid), pTimer(pTimer), pPDE(pPDE), pBUpdate(pBUpdate){
    pSource = new PointSource(pGrid);
}

Updater::~Updater(){
    delete pPDE;
    delete pBUpdate;
    delete pSource;
}

void Updater::exec(){
    // Makes use of directional splitting. Fluxes are computed and solved for in the
    // directions x, y, then z.
    double t  = pTimer->t();
    double dt = pTimer->dt();
    std::cout << "Time: " << t << std::endl;
    pBUpdate->exec();
    pSource->exec(t);
    pPDE->exec('x',dt); 
    pBUpdate->exec();
    pPDE->exec('y',dt);
    //pBUpdate->exec();
    //pPDE->exec('z',dt);
    pTimer->advance();
    return;
}

}// Namespace closure
#endif /* UPDATER_HPP */
