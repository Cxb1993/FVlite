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

class OperatorInitialisationManager : public CompositeOperator<OperatorInitialisation>{
public:
    OperatorInitialisationManager(){}
    virtual void init( Setting& cfg);
    void setup_boundary_geometry( Grid& grid);
    void fix_edges( Grid& grid);
};

REGISTER( Operator, InitialisationManager)

void OperatorInitialisationManager::init( Setting& cfg){
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
        pOperator->init( modCfg);
        add_element( pOperator);
    }
}

void OperatorInitialisationManager::setup_boundary_geometry(Grid& grid){
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

    int sizeX = grid.full_width(DIM_X);
    int sizeY = grid.full_width(DIM_Y);
    double dx = grid.ds(DIM_X);
    double dy = grid.ds(DIM_Y);

    BoundaryGeometry Boundary;

    for( int jj=1; jj<sizeY-1; jj++){
        y = grid.position(DIM_Y,jj);
        for( int ii=1; ii<sizeX-1; ii++){
            x = grid.position(DIM_X,ii);
            // Get level set at corners
            levelset_bl = grid.interpolate(x-0.5*dx,y-0.5*dy);
            levelset_br = grid.interpolate(x+0.5*dx,y-0.5*dy);
            levelset_tl = grid.interpolate(x-0.5*dx,y+0.5*dy);
            levelset_tr = grid.interpolate(x+0.5*dx,y+0.5*dy);
            // Calculate geometry parameters
            Boundary.set( dx, dy, levelset_bl, levelset_br, levelset_tl, levelset_tr);
            grid.boundary(ii,jj) = Boundary;
        }
    }
    fix_edges(grid);
    return;
}

void OperatorInitialisationManager::fix_edges( Grid& grid){
    // Errors are introduced if attempting to allow any solid to extend into edge boundaries.
    // This function fixes that.

    int startX = grid.start(DIM_X);
    int startY = grid.start(DIM_Y);
    int endX   = grid.end(DIM_X);
    int endY   = grid.end(DIM_Y);

    BoundaryGeometry BoundaryL, BoundaryR;
    double temp;
    Vector3 Nb;

    // Left/right boundary
    for( int jj=startY; jj<endY; jj++){
        BoundaryL = grid.boundary(startX,jj);
        BoundaryR = grid.boundary(endX-1,jj);

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

        grid.boundary(startX-1,jj) = BoundaryL;
        grid.boundary(endX,jj) = BoundaryR;
    }


    // Top/bottom boundary
    for( int ii=startX; ii<endX; ii++){
        BoundaryL = grid.boundary(ii,startY);
        BoundaryR = grid.boundary(ii,endY-1);

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

        grid.boundary(ii,startY-1) = BoundaryL;
        grid.boundary(ii,endY) = BoundaryR;
    }
}

}// Namespace closure
#endif /* OPERATORINITIALISATIONMANAGER_HPP */
