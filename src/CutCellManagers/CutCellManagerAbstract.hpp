// CutCellManagerAbstract.hpp
//
// Owned by the flux solver, updates fluxes to account for cut cells

#ifndef CUTCELLABSTRACT_HPP
#define CUTCELLABSTRACT_HPP


#include "Grid/Grid.hpp"

namespace FVlite{

class CutCellManager{

protected:

    Grid* pGrid;
    FluxSolver* pFlux;

public:

    CutCellManager(){}
    virtual ~CutCellManager(){}

    void init( Grid* pGrid, FluxSolver* pFlux); 

    virtual void newTimeStepSetup() = 0;
    virtual void correctFluxes( char dim, double dt)  = 0;

};

ObjectFactory<CutCellManager> CutCellManagerFactory;

void CutCellManager::init( Grid* pGrid, FluxSolver* pFlux){
    (*this).pGrid = pGrid;
    (*this).pFlux = pFlux;
    return;
}

}//namspace closure
#endif /* CUTCELLABSTRACT_HPP */
