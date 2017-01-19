// InitialiseCutCell.hpp
//
// Wraps OperatorInitialise
// Uses levelset values to determine cut cell boundary
// geometry.

#ifndef CUTCELLINITIALISATION_HPP
#define CUTCELLINITIALISATION_HPP

#include <libconfig.h++>

#include "OperatorInitialise.hpp"
#include "Operators/DecoratorOperator.hpp"

using libconfig::Setting;

namespace FVlite{

class OperatorInitialiseCutCell : public DecoratorOperator<OperatorInitialise>{
public:
    using DecoratorOperator::DecoratorOperator;
    virtual void init( Setting& cfg){ (void)cfg; }
    virtual void exec( Grid& grid, Timer& timer);
    
    void setup_boundary_geometry( Grid& grid);
    void fix_edges( Grid& grid);
};

void OperatorInitialiseCutCell::exec( Grid& grid, Timer& timer){
    DecoratorOperator<OperatorInitialise>::exec(grid,timer);
    setup_boundary_geometry(grid);
    fix_edges(grid);
}

void OperatorInitialiseCutCell::setup_boundary_geometry(Grid& grid){
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

    double dx = grid.ds(DIM_X);
    double dy = grid.ds(DIM_Y);

    unsigned int startX = grid.state_start(DIM_X);
    unsigned int startY = grid.state_start(DIM_Y);
    unsigned int endX = grid.state_end(DIM_X);
    unsigned int endY = grid.state_end(DIM_Y);
    unsigned int ls_startX = grid.levelset_start(DIM_X);
    unsigned int ls_startY = grid.levelset_start(DIM_Y);

    BoundaryGeometry Boundary;

    for( unsigned int jj=startY, jjL = ls_startY; jj<endY; jj++, jjL++){
        for( unsigned int ii=startX, iiL = ls_startX; ii<endX; ii++, iiL++){
            // Get level set at corners
            levelset_bl = grid.levelset(iiL,jjL);
            levelset_br = grid.levelset(iiL+1,jjL);
            levelset_tl = grid.levelset(iiL,jjL+1);
            levelset_tr = grid.levelset(iiL+1,jjL+1);
            // Calculate geometry parameters
            Boundary.set( dx, dy, levelset_bl, levelset_br, levelset_tl, levelset_tr);
            grid.boundary(ii,jj) = Boundary;
        }
    }
}

void OperatorInitialiseCutCell::fix_edges( Grid& grid){
    // Errors are introduced if attempting to allow any solid to extend into edge boundaries.
    // This function fixes that.
    // TODO Generalise this

    unsigned int startX = grid.state_start(DIM_X);
    unsigned int startY = grid.state_start(DIM_Y);
    unsigned int endX   = grid.state_end(DIM_X);
    unsigned int endY   = grid.state_end(DIM_Y);

    BoundaryGeometry BoundaryL, BoundaryR;
    double temp;
    Vector3 Nb;

    // Left/right boundary
    for( unsigned int jj=startY; jj<endY; jj++){
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
    for( unsigned int ii=startX; ii<endX; ii++){
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

}
#endif
