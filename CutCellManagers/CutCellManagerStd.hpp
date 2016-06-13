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
    virtual void correctFluxes( char dim, double dt);

    // Component functions used in cut cell scheme
    double getAlphaShielded( const BoundaryGeometry& Boundary, char dim);
    FluxVector getModifiedFlux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR, const StateVector& AuxL, const StateVector& AuxR, const BoundaryGeometry& BL, const BoundaryGeometry& BR);
};

REGISTER(CutCellManager,Std)


void CutCellManagerStd::newTimeStepSetup(){
    // TODO reflective conditions are currently hardcoded. Change this.
#ifdef DEBUG
    std::cout << "NEW TIME STEP" << std::endl;
#endif
    int startX=pGrid->startX();
    int startY=pGrid->startY();
    int endX=pGrid->endX();
    int endY=pGrid->endY();
    StateVector State;
    StateVector BoundaryState;
    Vector3 Velocity;
    Vector3 VelocityR;
    Vector3 WaveSpeeds;
    Vector3 Normal;
    Velocity[2]=0; VelocityR[2]=0; Normal[2]=0;
    double angle;
    BoundaryGeometry Boundary;
    for( int jj=startY; jj<endY; jj++){
        for( int ii=startX; ii<endX; ii++){
            Boundary = pGrid->boundary(ii,jj);
            if(Boundary.alpha() == 1. || Boundary.alpha() == 0.) continue;
            
            State = pGrid->state(ii,jj);

            // Step I
            // Find rotation matrix to convert velocity to tangential/normal frame
            Normal = Boundary.Nb();
            angle = atan2( Normal[1], Normal[0]);
            if( angle < 0. ) angle += 360/M_PI;

            // Step II
            // Rotate StateVector into that frame
            Velocity = State.getVelocity();
            VelocityR[0] =  Velocity[0]*cos(angle) - Velocity[1]*sin(angle);
            VelocityR[1] =  Velocity[0]*sin(angle) + Velocity[1]*cos(angle);
            State.set(State.rho(),VelocityR[0],VelocityR[1],State.p());
            
            // Step III
            // Get appropriate boundary state
            BoundaryState = State;
            BoundaryState[1] = -State[1]; // Set x speed negative in new coordinates
            //BoundaryState[2] = -State[2]; // Set y speed negative in new coordinates

            // Step IV
            // Solve Riemann problem at boundary
            WaveSpeeds = pFlux->getWaveSpeeds('x',State,BoundaryState);
            State = pFlux->getHLLCstate('x',State,BoundaryState,WaveSpeeds[0],WaveSpeeds[1],WaveSpeeds[2]);


            // Step V
            // Rotate back to standard frame
            //Velocity = State.getVelocity();
            //VelocityR[0] =  Velocity[0]*cos(angle) + Velocity[1]*sin(angle);
            //VelocityR[1] = -Velocity[0]*sin(angle) + Velocity[1]*cos(angle);
            //State.set(State.rho(),VelocityR[0],VelocityR[1],State.p());

            // Step V alt
            // Retain only tangential component of velocity at interface
            // (from Nandan's code)
            Velocity = Velocity - (Velocity*Normal)*Normal;
            State.set(State.rho(),Velocity[0],Velocity[1],State.p());

            // Step VI
            // Store result in reference state
            pGrid->state_ref(ii,jj) = State;
        }
    }

    return;
}

void CutCellManagerStd::correctFluxes( char dim, double dt){
    int startX=pGrid->startX();
    int startY=pGrid->startY();
    int endX=pGrid->endX();
    int endY=pGrid->endY();
    double alphaL, alphaR, betaC;
    BoundaryGeometry BoundaryL, BoundaryR;
    StateVector StateL, StateR, StateAuxL, StateAuxR;
    FluxVector Flux;
    double ds;
    switch(dim){
        case 'x':
        ds = pGrid->dx();
        for( int jj=startY; jj<endY; jj++){
            for( int ii=startX-1; ii<endX; ii++){
                BoundaryL = pGrid->boundary(ii,jj);
                BoundaryR = pGrid->boundary(ii+1,jj);
                alphaL = BoundaryL.alpha();
                alphaR = BoundaryR.alpha();
                betaC  = BoundaryL.betaR();
                if( (alphaL>0. && alphaR>0.) && (alphaL<1. || alphaR<1.) && (betaC > 0.) ){
                    StateL = pGrid->state(ii,jj);
                    StateR = pGrid->state(ii+1,jj);
                    StateAuxL = pGrid->state_ref(ii,jj);
                    StateAuxR = pGrid->state_ref(ii+1,jj);
                    Flux = getModifiedFlux(ds,dt,dim,StateL,StateR,StateAuxL,StateAuxR,BoundaryL,BoundaryR);
#ifdef DEBUG
                    if( Flux.isnan()){
                        std::cerr << "Modified Flux("<<ii<<","<<jj<<") is nan" << std::endl;
                    }
#endif
                    pGrid->flux(ii,jj) = Flux;
                }
            }
        }
        break;
        case 'y':
        ds = pGrid->dy();
        for( int jj=startY-1; jj<endY; jj++){
            for( int ii=startX; ii<endX; ii++){
                BoundaryL = pGrid->boundary(ii,jj);
                BoundaryR = pGrid->boundary(ii,jj+1);
                alphaL = BoundaryL.alpha();
                alphaR = BoundaryR.alpha();
                betaC  = BoundaryL.betaT();
                if( (alphaL>0. && alphaR>0.) && (alphaL<1. || alphaR<1.) && (betaC > 0.) ){
                    StateL = pGrid->state(ii,jj);
                    StateR = pGrid->state(ii,jj+1);
                    StateAuxL = pGrid->state_ref(ii,jj);
                    StateAuxR = pGrid->state_ref(ii,jj+1);
                    Flux = getModifiedFlux(ds,dt,dim,StateL,StateR,StateAuxL,StateAuxR,BoundaryL,BoundaryR);
#ifdef DEBUG
                    if( Flux.isnan()){
                        std::cerr << "Modified Flux("<<ii<<","<<jj<<") is nan" << std::endl;
                    }
#endif
                    pGrid->flux(ii,jj) = Flux;
                }
            }
        }
        break;
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

FluxVector CutCellManagerStd::getModifiedFlux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR, const StateVector& AuxL, const StateVector& AuxR, const BoundaryGeometry& BL, const BoundaryGeometry& BR){

    FluxVector Shielded, Unshielded, BoundaryFlux, Modified;
    double alpha_shielded;
    double betaL,betaR,betaC;

    // If y sweep, exchange betas so it looks like an x sweep
    switch(dim){
        case 'x':
            betaL = BL.betaL();
            betaC = BL.betaR();
            betaR = BR.betaR();
#ifdef DEBUG
            if( fabs(betaC-BR.betaL()) > 1e-2){
                std::cerr << "Beta error, x sweep" << std::endl;
            }
#endif
            break;
        case 'y':
            betaL = BL.betaB();
            betaC = BL.betaT();
            betaR = BR.betaT();
#ifdef DEBUG
            if( fabs(betaC-BR.betaB()) > 1e-2){
                std::cerr << "Beta error, y sweep" << std::endl;
            }
#endif
            break;
        default:
            std::cerr <<"ERROR: bad dim, getModifiedFlux" << std::endl;
            betaL=betaC=betaR=0;
            exit(EXIT_FAILURE);
    }

    //Step 1
    //Compute 'unshielded flux'. This should be a first order HLLC flux between the two states
    Unshielded = pFlux->getIntercellFlux( ds,dt,dim,UL,UR);
#ifdef DEBUG
    if( Unshielded.isnan()){
        std::cerr << "Unshielded Flux is nan" << std::endl;
        std::cerr << "UL( "<<UL[0]<<", "<<UL[1]<<", "<<UL[2]<<", "<<UL[3]<<")"<< std::endl;
        std::cerr << "UR( "<<UR[0]<<", "<<UR[1]<<", "<<UR[2]<<", "<<UR[3]<<")"<< std::endl;
        std::cerr << "F( "<<Unshielded[0]<<", "<<Unshielded[1]<<", "<<Unshielded[2]<<", "<<Unshielded[3]<<")"<< std::endl;
    }
#endif

    //Step 1.1
    //Deal with case where the boundary comes to a point at the cell boundary:
    //-----------------------
    //|          |          |
    //|         /|\         |
    //|        / | \        |
    //|       /  |  \       |
    //-----------------------
    //(Equally applies to case where the boundary is flat)
    if( betaC <= betaL && betaC <= betaR){
        // Written in this way for consistency
        Modified = (betaC*Unshielded)/betaC;
#ifdef DEBUG
        if( Modified.isnan()){
            std::cerr << "Modified Flux is nan (from boundary peak)" << std::endl;
        }
#endif
        return Modified;
    }

    //Step 1.2
    //'Deal' with concave problem
    if( betaC > betaL &&  betaC > betaR){
        std::cerr << "ERROR: Encountered concave boundary. Cut cell algorithm failed." << std::endl;
        exit(EXIT_FAILURE);
    }

    //Step 1.3
    //'Deal' with no boundary in the middle
    if( betaC == 0.){
        std::cerr << "ERROR: Middle boundary is zero. Cut cell algorithm failed." << std::endl;
        exit(EXIT_FAILURE);
    }   

    //Step 2
    //Compute boundary flux
    //First, need to determine, whether left or right boundary flux is needed.
    bool positive_slope = (betaR<betaL) ? true : false;
    if(positive_slope){
        BoundaryFlux.set(AuxR,dim);
    } else {
        BoundaryFlux.set(AuxL,dim);
    }
#ifdef DEBUG
    if( BoundaryFlux.isnan()){
        std::cerr << "BoundaryFlux is nan" << std::endl;
        if(positive_slope){
            std::cerr << "AuxR" << std::endl;
            std::cerr << AuxR[0] << std::endl;
            std::cerr << AuxR[1] << std::endl;
            std::cerr << AuxR[2] << std::endl;
            std::cerr << AuxR[3] << std::endl;
        } else {
            std::cerr << "AuxL" << std::endl;
            std::cerr << AuxL[0] << std::endl;
            std::cerr << AuxL[1] << std::endl;
            std::cerr << AuxL[2] << std::endl;
            std::cerr << AuxL[3] << std::endl;
        }
    }
#endif

    //Step 3
    //Compute shielded flux
    //a) get alpha shielded
    if(positive_slope){
       alpha_shielded = getAlphaShielded(BR,dim);
    } else {
        alpha_shielded = getAlphaShielded(BL,dim);
    }
    //b) get F shielded
    Shielded = BoundaryFlux + alpha_shielded*(Unshielded-BoundaryFlux);
#ifdef DEBUG
    if( Shielded.isnan()){
        std::cerr << "Shielded Flux is nan" << std::endl;
    }
#endif

    //Step 4
    //Compute modified flux
    if(positive_slope){
        Modified = (betaR*Unshielded + (betaC-betaR)*Shielded)/betaC;
    } else {
        Modified = (betaL*Unshielded + (betaC-betaL)*Shielded)/betaC;
    }
#ifdef DEBUG
    if( Modified.isnan()){
        std::cerr << "Modified Flux is nan" << std::endl;
    }
#endif

    return Modified;
}

}//namspace closure
#endif /* CUTCELLSTD_HPP */
