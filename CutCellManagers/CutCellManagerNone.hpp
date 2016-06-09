// CutCellManagerNone.hpp
//
// Owned by the flux solver, doesn't do anything!

#ifndef CUTCELLNONE_HPP
#define CUTCELLNONE_HPP

#include "CutCellManagerAbstract.hpp"

namespace FVlite{

class CutCellManagerNone : public CutCellManager {
    virtual void newTimeStepSetup(){return;}
    virtual void correctFluxes( char dim, double dt){(void)dim;(void)dt;return;}
};

REGISTER(CutCellManager,None)

}//namspace closure
#endif /* CUTCELLNONE_HPP */
