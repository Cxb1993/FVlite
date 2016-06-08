// CutCellManagerStd.hpp
//
// Owned by the flux solver, updates intercell fluxes to account for cut cells.
// Method based on Klein, 2009.

#ifndef CUTCELLSTD_HPP
#define CUTCELLSTD_HPP

#include "CutCellManagerAbstract.hpp"

namespace FVlite{

class CutCellManagerStd : public CutCellManager {
    virtual void newTimeStepSetup();
    virtual void correctFluxes(){return;}

    // Component functions used in cut cell scheme
    double getAlphaShielded( const BoundaryGeometry& Boundary, char dim);
    FluxVector getBoundaryFlux( int ii, int jj, char dim);
    FluxVector getShieldedFlux( int ii, int jj, char dim);
    FluxVector getAuxFlux( int ii, int jj, char dim);
    FluxVector getModifiedFlux( int ii, int jj, char dim);
};

REGISTER(CutCellManager,Std)


    // WARNING, CODE INCORRECT
void CutCellManagerStd::newTimeStepSetup(){
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


double CutCellManagerStd::getAlphaShielded( const BoundaryGeometry& Boundary, char dim){
    // Case 1
    // Cuts both sides
    // --------------
    // |            |
    // | __________/|
    // |/           |
    // |            |
    // --------------
    // In this case, alpha shielded is always going to be 0.5. Or average of betas.
    //
    // Case 2
    // Triangular cut
    // --------------
    // |  /         |
    // | /          |
    // |/           |
    // |            |
    // --------------
    // In this case, find average of opposite betas.
    //
    // Case 3
    // Cuts opposite sides
    // --------------
    // |      /     |
    // |     /      |
    // |    /       |
    // |   /        |
    // --------------
    // In this case, again find average of opposite betas
    //
    // We see that in all cases, alpha shielded is just the average of opposite betas.
    double beta1, beta2;
    switch(dim){
        case 'x':
            beta1 = Boundary.betaT();
            beta2 = Boundary.betaB();
            break;
        case 'y':
            beta1 = Boundary.betaL();
            beta2 = Boundary.betaR();
            break;
        default:
            // This code should never be called
            beta1=0.; beta2=0.;
            std::cerr << "ERROR: incorrect dim, cut cell alpha shielded" << std::endl;
            exit(EXIT_FAILURE);
    }
    return 0.5*(beta1+beta2);
}

//    FluxVector getShieldedFlux( int ii, int jj, char dim);
//    FluxVector getAuxFlux( int ii, int jj, char dim);
//    FluxVector getBoundaryFlux( int ii, int jj, char dim);

//FluxVector CutCellManagerStd::getModifiedFlux( int ii, int jj, char dim){


}//namspace closure
#endif /* CUTCELLSTD_HPP */
