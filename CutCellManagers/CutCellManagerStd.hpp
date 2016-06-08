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



/*
void FVMsolverCutCell::newTimeStep(){
    // Copies all states into reference states
    // TODO only do this for cut cells
    int startX=pGrid->startX();
    int startY=pGrid->startY();
    int endX=pGrid->endX();
    int endY=pGrid->endY();
    StateVector State;
    Vector3 Velocity;
    Vector3 Tangential;
    BoundaryGeometry Boundary;
    for( int jj=startY; jj<endY; jj++){
        for( int ii=startX; ii<endX; ii++){
            Boundary = pGrid->boundary(ii,jj);
            if(Boundary.alpha() == 1.) continue;
            State = pGrid->state(ii,jj);
            Velocity = State.getVelocity();
            Tangential = Velocity - Velocity*Boundary.Nb();
            State.set(State.rho(),Tangential[0],Tangential[1],State.p());
            pGrid->state_ref(ii,jj) = State;
        }
    }
    return;
}
*/

}//namspace closure
#endif /* CUTCELLSTD_HPP */
