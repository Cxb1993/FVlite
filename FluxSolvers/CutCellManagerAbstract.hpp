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

public:

    CutCellManager(){}
    virtual CutCellManager(){}

    void init( Grid* pGrid){ (*this).pGrid=pGrid;}

    virtual void newTimeStepSetup() = 0;
    virtual void correctFluxes()    = 0;

};

ObjectFactory<CutCellManager> CutCellManagerFactory;

}//namspace closure
#endif /* CUTCELLABSTRACT_HPP */
