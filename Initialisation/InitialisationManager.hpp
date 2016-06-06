// InitialisationManager.hpp
//
// Coordinates initialisation modules.

#ifndef INITIALISATIONMANAGER_HPP
#define INITIALISATIONMANAGER_HPP

#include <list>
#include <libconfig.h++>

#include "Utils/GridManager.hpp"
#include "InitialisationModules.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class InitialisationManager : public GridManager<InitialisationModule>{
protected:
    Grid* pGrid;
public:
    InitialisationManager(){}
    virtual void init( Grid* pGrid, Setting& cfg);
    void setup_boundary_geometry();
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
        add_module( pImod);
    }
    sort_modules();
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
            pGrid->boundary(ii,jj).set( dx, dy, levelset_bl, levelset_br, levelset_tl, levelset_tr);
        }
    }

    return;
}

}// Namespace closure
#endif /* INITIALISATIONMANAGER_HPP */
