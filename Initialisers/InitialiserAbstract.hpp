// InitialiserAbstract.hpp
//
// Abstract finite volume grid initialisaer.

#ifndef INITIALISERABSTRACT_HPP
#define INITIALISERABSTRACT_HPP

#include "Utils/ObjectFactory.hpp"

#include "Vectors/Grid.hpp"
#include "Timer/Timer.hpp"

namespace FVTD{

// Class delcaration

class Initialiser{

protected:

    Grid* pGrid;

public:

    Initialiser(){}
    virtual ~Initialiser(){}

    void init( Grid* pGrid);
    virtual void exec() = 0;
    void setup_boundary_geometry();
};

// Factory declaration

ObjectFactory<Initialiser> InitialiserFactory;

// Function definitions

void Initialiser::init( Grid* pGrid){
    (*this).pGrid = pGrid;
    return;
}

void Initialiser::setup_boundary_geometry(){
    
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
#endif /* INITIALISERABSTRACT_HPP */
