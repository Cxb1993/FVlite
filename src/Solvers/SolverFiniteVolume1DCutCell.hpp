// SolverFiniteVolume1DCutCell.hpp
//
// Variation on 1D finite volume method which is compatible
// with cut cell methods.

#ifndef SOLVERFINITEVOLUME1DCUTCELL_HPP
#define SOLVERFINITEVOLUME1DCUTCELL_HPP

#include <iostream>
#include <cstdlib>

#include "SolverFiniteVolume1D.hpp"
#include "CutCellManager/CutCellManager.hpp"

namespace FVlite{

class SolverFiniteVolume1DCutCell : public SolverFiniteVolume1D{
protected:
    CutCellManager* mpCutCell;
public:
    virtual ~SolverFiniteVolume1DCutCell();
    virtual void init( Grid* pGrid, Setting& cfg);
    virtual void exec( char dim, double dt);
    virtual void newTimeStep(){
        mpCutCell->newTimeStepSetup();
    }
};

REGISTER(Solver,FiniteVolume1DCutCell)

SolverFiniteVolume1DCutCell::~SolverFiniteVolume1DCutCell(){
    delete mpCutCell;
}

void SolverFiniteVolume1DCutCell::init( Grid* pGrid, Setting& cfg){
    SolverFiniteVolume1D::init(pGrid,cfg);
    mpCutCell = new CutCellManager;
    mpCutCell->init(pGrid,mpFlux);
}

void SolverFiniteVolume1DCutCell::exec( char dim, double dt){
    int startX = mpGrid->startX();
    int startY = mpGrid->startY();
    int endX = mpGrid->endX();
    int endY = mpGrid->endY();
    double ds;
    StateVector State;
    BoundaryGeometry Boundary;
    FluxVector FluxL,FluxR,BoundaryFlux;
    double alpha, betaL, betaR;

    // Method:
    // 1) Sweep grid.
    // 2) If cell is filled (alpha=0), skip.
    // 3) If cell is free (alpha=1), standard explicit update.
    // 4) If cell is mixed, apply cut-cell update

    switch(dim){
        case 'x' :
#ifdef DEBUG
            std::cout<< "Beginning X sweep" << std::endl;
#endif
            ds = mpGrid->dx();
            // Solve flux
            mpFlux->exec(dim,dt);
            mpCutCell->correctFluxes(dim,dt);
            // Explicit update formula -- Euler method
            for( int jj=startY; jj<endY; jj++){
                for( int ii=startX; ii<endX; ii++){
                    Boundary = mpGrid->boundary(ii,jj);
                    alpha = Boundary.alpha();
                    betaL = Boundary.betaL();
                    betaR = Boundary.betaR();
                    if( alpha == 0.) continue;
                    if( alpha == 1.){
                        FluxL = mpGrid->flux(ii-1,jj);
                        FluxR = mpGrid->flux(ii,jj);
                        mpGrid->state(ii,jj) = mpGrid->state(ii,jj) + (FluxL-FluxR) * dt/ds;
                    }else{
                        BoundaryFlux.set( mpGrid->state_ref(ii,jj),dim);
                        FluxL = mpGrid->flux(ii-1,jj);
                        FluxR = mpGrid->flux(ii,jj);
//                        if( ii==600){
//                            std::cout << std::endl;
//                            std::cout << "FluxL= " << "("<<FluxL[0]<<", "<<FluxL[1]<<", "<<FluxL[2]<<", "<<FluxL[3]<<", "<<FluxL[4]<<", "<<FluxL[5]<< ")"<<std::endl;
//                            std::cout << "FluxR= " << "("<<FluxR[0]<<", "<<FluxR[1]<<", "<<FluxR[2]<<", "<<FluxR[3]<<", "<<FluxR[4]<<", "<<FluxR[5]<<")"<<std::endl;
//                            std::cout << "BoundaryFlux= " << "("<<BoundaryFlux[0]<<", "<<BoundaryFlux[1]<<", "<<BoundaryFlux[2]<<", "<<BoundaryFlux[3]<<", "<<BoundaryFlux[4]<<", "<<BoundaryFlux[5] << ")"<<std::endl;
//                        }
                        mpGrid->state(ii,jj) += (betaL*FluxL - betaR*FluxR - (betaL-betaR)*BoundaryFlux) * dt/(ds*alpha);
                        
#if 0
                        if( pGrid->state(ii,jj).Hz() > 10.0){
                            std::cout << "("<<ii<<","<<jj<<")"<<std::endl;
                            std::cout << "alpha= " << alpha << std::endl;
                            std::cout << "betaL= " << betaL << std::endl;
                            std::cout << "betaR= " << betaR << std::endl;
                            std::cout << "FluxL= " << "("<<FluxL[0]<<", "<<FluxL[1]<<", "<<FluxL[2]<<", "<<FluxL[3]<<", "<<FluxL[4]<<", "<<FluxL[5]<< ")"<<std::endl;
                            std::cout << "FluxR= " << "("<<FluxR[0]<<", "<<FluxR[1]<<", "<<FluxR[2]<<", "<<FluxR[3]<<", "<<FluxR[4]<<", "<<FluxR[5]<<")"<<std::endl;
                            std::cout << "BoundaryState= " << "("<<pGrid->state_ref(ii,jj)[0]<<", "<<pGrid->state_ref(ii,jj)[1]<<", "<<pGrid->state_ref(ii,jj)[2]<<", "<<pGrid->state_ref(ii,jj)[3]<<", " << pGrid->state_ref(ii,jj)[4]<<", "<<pGrid->state_ref(ii,jj)[5]<<")"<<std::endl;
                            std::cout << "BoundaryFlux= " << "("<<BoundaryFlux[0]<<", "<<BoundaryFlux[1]<<", "<<BoundaryFlux[2]<<", "<<BoundaryFlux[3]<<", "<<BoundaryFlux[4]<<", "<<BoundaryFlux[5] << ")"<<std::endl;
                        }
#endif                   
                    }
                }
            }
#ifdef DEBUG
            std::cout<< "Finished X sweep" << std::endl;
#endif
            break;
        case 'y' :
#ifdef DEBUG
            std::cout<< "Beginning Y sweep" << std::endl;
#endif
            ds = mpGrid->dy();
            // Solve flux
            mpFlux->exec(dim,dt);
            mpCutCell->correctFluxes(dim,dt);
            // Explicit update formula -- Euler method
            for( int jj=startY; jj<endY; jj++){
                for( int ii=startX; ii<endX; ii++){
                    Boundary = mpGrid->boundary(ii,jj);
                    alpha = Boundary.alpha();
                    betaL = Boundary.betaB();
                    betaR = Boundary.betaT();
                    if( alpha == 0.) continue;
                    if( alpha == 1.){
                        mpGrid->state(ii,jj) = mpGrid->state(ii,jj) + (mpGrid->flux(ii,jj-1)-mpGrid->flux(ii,jj)) * dt/ds;
                    }else{
                        BoundaryFlux.set( mpGrid->state_ref(ii,jj),dim);
                        FluxL = mpGrid->flux(ii,jj-1);
                        FluxR = mpGrid->flux(ii,jj);
                        mpGrid->state(ii,jj) += (betaL*FluxL - betaR*FluxR - (betaL-betaR)*BoundaryFlux) * dt/(ds*alpha);
                    }
                }
            }
#ifdef DEBUG
            std::cout<< "Finished Y sweep" << std::endl;
#endif
            break;
        case 'z' :
            std::cerr << "Error, z direction not implemented" << std::endl;
            exit(EXIT_FAILURE);
            break;
        default:
            std::cerr << "Error, invalid direction specifier" << std::endl;
            exit(EXIT_FAILURE);
    }
    (void)betaL;
    (void)betaR;
    (void)ds;
    return;
}


}// Namespace closure
#endif /* SOLVERFINITEVOLUME1DCUTCELL_HPP */
