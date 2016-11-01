// InitialisationManager.hpp
//
// Coordinates initialisation modules.

#ifndef INITIALISATIONMANAGER_HPP
#define INITIALISATIONMANAGER_HPP

#include <list>
#include <libconfig.h++>

#include "Utils/Composite.hpp"
#include "InitialisationModules.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class InitialisationManager : public Composite<InitialisationModule>{
protected:
    Grid* pGrid;
public:
    InitialisationManager(){}
    virtual void init( Grid* pGrid, Setting& cfg);
    void setup_boundary_geometry();
    void fix_edges();
};

void InitialisationManager::init( Grid* pGrid, Setting& cfg){
    (*this).pGrid = pGrid;
    int nMods = cfg.getLength();
    InitialisationModule* pImod;
    string modType;
    for( int count=0; count<nMods; count++){
        Setting& modCfg = cfg[count];
        modType = modCfg.lookup("type").c_str();
        pImod = InitialisationModuleFactory.create( modType);
        pImod->init( pGrid, modCfg);
        add_element( pImod);
    }
    return;
}

void InitialisationManager::setup_boundary_geometry(){
    
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

    int sizeX = pGrid->sizeX();
    int sizeY = pGrid->sizeY();
    double dx = pGrid->dx();
    double dy = pGrid->dy();

    BoundaryGeometry Boundary;

    for( int jj=1; jj<sizeY-1; jj++){
        y = pGrid->y(jj);
        for( int ii=1; ii<sizeX-1; ii++){
            x = pGrid->x(ii);
            // Get level set at corners
            levelset_bl = pGrid->levelset()->interpolate(x-0.5*dx,y-0.5*dy);
            levelset_br = pGrid->levelset()->interpolate(x+0.5*dx,y-0.5*dy);
            levelset_tl = pGrid->levelset()->interpolate(x-0.5*dx,y+0.5*dy);
            levelset_tr = pGrid->levelset()->interpolate(x+0.5*dx,y+0.5*dy);
            // Calculate geometry parameters
            Boundary.set( dx, dy, levelset_bl, levelset_br, levelset_tl, levelset_tr);
            pGrid->boundary(ii,jj) = Boundary;
        }
    }
    fix_edges();
    return;
}

void InitialisationManager::fix_edges(){
    // Errors are introduced if attempting to allow any solid to extend into edge boundaries.
    // This function fixes that.

    int startX = pGrid->startX();
    int startY = pGrid->startY();
    int endX   = pGrid->endX();
    int endY   = pGrid->endY();

    BoundaryGeometry BoundaryL, BoundaryR;
    double temp;
    Vector3 Nb;

    // Left/right boundary
    for( int jj=startY; jj<endY; jj++){
        BoundaryL = pGrid->boundary(startX,jj);
        BoundaryR = pGrid->boundary(endX-1,jj);

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

        pGrid->boundary(startX-1,jj) = BoundaryL;
        pGrid->boundary(endX,jj) = BoundaryR;
    }


    // Top/bottom boundary
    for( int ii=startX; ii<endX; ii++){
        BoundaryL = pGrid->boundary(ii,startY);
        BoundaryR = pGrid->boundary(ii,endY-1);

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

        pGrid->boundary(ii,startY-1) = BoundaryL;
        pGrid->boundary(ii,endY) = BoundaryR;
    }
}

}// Namespace closure
#endif /* INITIALISATIONMANAGER_HPP */
