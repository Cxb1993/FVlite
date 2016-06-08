// CutCellManagerStd.hpp
//
// Owned by the flux solver, updates intercell fluxes to account for cut cells.
// Method based on Klein, 2009.

#ifndef CUTCELLSTD_HPP
#define CUTCELLSTD_HPP

#include "CutCellManagerAbstract.hpp"

namespace FVlite{

class CutCellManagerStd : public CutCellManager {
    virtual void newTimeStepSetup(){return;}
    virtual void correctFluxes(){return;}
};

REGISTER(CutCellManager,Std)

}//namspace closure
#endif /* CUTCELLSTD_HPP */
