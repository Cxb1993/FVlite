// InitialisationManager.hpp
//
// Coordinates initialisation modules.

#ifndef INITIALISATIONMANAGER_HPP
#define INITIALISATIONMANAGER_HPP

#include <list>
#include <libconfig.h++>

#include "Utils/Composite.hpp"
#include "OperatorInitialisation.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class OperatorInitialisationManager : public Composite<OperatorInitialisation>{
public:
    OperatorInitialisationManager(){}
    virtual void init( Grid* pGrid, Timer* pTimer,  Setting& cfg);
    void setup_boundary_geometry();
    void fix_edges();
};

REGISTER( Operator, InitialisationManager)

void OperatorInitialisationManager::init( Grid* pGrid, Timer* pTimer, Setting& cfg){
    Operator::init(pGrid,pTimer,cfg);
    int nMods = cfg.getLength();
    OperatorInitialisation* pOperator;
    string modType;
    for( int count=0; count<nMods; count++){
        Setting& modCfg = cfg[count];
        try{
            modType = modCfg.lookup("type").c_str();
        } catch( const std::exception& e){
            // skip if other information found
            continue;
        }
        pOperator = new OperatorInitialisation;
        pOperator->init( pGrid, pTimer, modCfg);
        add_element( pOperator);
    }
}

void OperatorInitialisationManager::setup_boundary_geometry(){
    
    // Scan through grid, use level set function to determine geometry at boundaries.
    // Calculation at each cell performed by BoundaryGeometry class.
    // Notation:
    //  > t : top
    //  > b : bottom
    //  > l : left
    //  > r : right

    double levelset_bl;
    double levelset_br;
    double levelset_tl;
    double levelset_tr;

    double x,y; // cell center locations

    int sizeX = mpGrid->sizeX();
    int sizeY = mpGrid->sizeY();
    double dx = mpGrid->dx();
    double dy = mpGrid->dy();

    BoundaryGeometry Boundary;

    for( int jj=1; jj<sizeY-1; jj++){
        y = mpGrid->y(jj);
        for( int ii=1; ii<sizeX-1; ii++){
            x = mpGrid->x(ii);
            // Get level set at corners
            levelset_bl = mpGrid->levelset()->interpolate(x-0.5*dx,y-0.5*dy);
            levelset_br = mpGrid->levelset()->interpolate(x+0.5*dx,y-0.5*dy);
            levelset_tl = mpGrid->levelset()->interpolate(x-0.5*dx,y+0.5*dy);
            levelset_tr = mpGrid->levelset()->interpolate(x+0.5*dx,y+0.5*dy);
            // Calculate geometry parameters
            Boundary.set( dx, dy, levelset_bl, levelset_br, levelset_tl, levelset_tr);
            mpGrid->boundary(ii,jj) = Boundary;
        }
    }
    fix_edges();
    return;
}

void OperatorInitialisationManager::fix_edges(){
    // Errors are introduced if attempting to allow any solid to extend into edge boundaries.
    // This function fixes that.

    int startX = mpGrid->startX();
    int startY = mpGrid->startY();
    int endX   = mpGrid->endX();
    int endY   = mpGrid->endY();

    BoundaryGeometry BoundaryL, BoundaryR;
    double temp;
    Vector3 Nb;

    // Left/right boundary
    for( int jj=startY; jj<endY; jj++){
        BoundaryL = mpGrid->boundary(startX,jj);
        BoundaryR = mpGrid->boundary(endX-1,jj);

        temp = BoundaryL.betaL();
        BoundaryL.betaL() = BoundaryL.betaR();
        BoundaryL.betaR() = temp;
        Nb = BoundaryL.Nb();
        Nb[1] = -Nb[1];
        BoundaryL.Nb() = Nb;

        temp = BoundaryR.betaL();
        BoundaryR.betaL() = BoundaryR.betaR();
        BoundaryR.betaR() = temp;
        Nb = BoundaryR.Nb();
        Nb[1] = -Nb[1];
        BoundaryR.Nb() = Nb;

        mpGrid->boundary(startX-1,jj) = BoundaryL;
        mpGrid->boundary(endX,jj) = BoundaryR;
    }


    // Top/bottom boundary
    for( int ii=startX; ii<endX; ii++){
        BoundaryL = mpGrid->boundary(ii,startY);
        BoundaryR = mpGrid->boundary(ii,endY-1);

        temp = BoundaryL.betaB();
        BoundaryL.betaB() = BoundaryL.betaT();
        BoundaryL.betaT() = temp;
        Nb = BoundaryL.Nb();
        Nb[0] = -Nb[0];
        BoundaryL.Nb() = Nb;

        temp = BoundaryR.betaB();
        BoundaryR.betaB() = BoundaryR.betaT();
        BoundaryR.betaT() = temp;
        Nb = BoundaryR.Nb();
        Nb[0] = -Nb[0];
        BoundaryR.Nb() = Nb;

        mpGrid->boundary(ii,startY-1) = BoundaryL;
        mpGrid->boundary(ii,endY) = BoundaryR;
    }
}

}// Namespace closure
#endif /* OPERATORINITIALISATIONMANAGER_HPP */
