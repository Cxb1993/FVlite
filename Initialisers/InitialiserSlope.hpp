// InitialiserSlope.hpp
//
// Finite volume grid initialisaer.
// Initialises zeroes except for PEC slope to RHS of grid.

#ifndef INITIALISERSLOPE_HPP
#define INITIALISERSLOPE_HPP

#include"InitialiserAbstract.hpp"

#include <cmath>

namespace FVTD{

class InitialiserSlope : public Initialiser{
public:
    InitialiserSlope( Grid* pGrid) : Initialiser(pGrid) {}
    virtual void exec();
    void cut_cell_params( double x, double y, double slope_gradient, double slope_intersect);
};

void InitialiserSlope::exec(){

    double x, y;
    // Line defined by two points (x1,y1) and (x2,y2)
    double x1 = 0.5;
    double y1 = 0.;
    double x2 = 1.;
    double y2 = 0.3333333333333;
    double distance;

#ifdef EULER
    StateVector LEFT( 1.0, 0.75, 0.0, 1.0);
    StateVector RIGHT( 0.125, 0.0, 0.0, 0.1);
    double left_pos = 0.1;
#endif

    int startX = pGrid->startX();
    int endX = pGrid->endX();
    int startY = pGrid->startY();
    int endY = pGrid->endY();
    int sizeX = pGrid->Nx() + 2*pGrid->bound();
    int sizeY = pGrid->Ny() + 2*pGrid->bound();

    for( int jj=0; jj<sizeY; jj++){
        y = pGrid->y(jj);
        for( int ii=0; ii<sizeX; ii++){
            x = pGrid->x(ii);
            if( ii>=startX && ii<endX && jj>=startY && jj<endY){
#ifdef EULER
                if( x < left_pos){
                    pGrid->state(ii,jj) = LEFT;
                } else {
                    pGrid->state(ii,jj) = RIGHT;
                }
#else
                pGrid->state(ii,jj)    = ZEROSTATE;
#endif
            }
            // Level set -- signed distance function
            distance = ((y2-y1)*x - (x2-x1)*y + x2*y1 - y2*x1)/sqrt((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1));
            pGrid->levelset()->workspace(ii,jj) = distance;
        }
    }
    pGrid->levelset()->merge();
    setup_boundary_geometry();
    return;
}

void InitialiserSlope::cut_cell_params( double x, double y, double m /*gradient*/, double c /*intersect*/){
#if 0
    // BoundaryGeometry parameters to derive
    double alpha;
    double betaXm;
    double betaXp;
    double betaYm;
    double betaYp;

    // Cell lengths
    double dx = pGrid->dx();
    double dy = pGrid->dy();

    // Cell corner coordinates
    double xmin=x-0.5*dx; 
    double xmax=x+0.5*dx;
    double ymin=y-0.5*dy;
    double ymax=y+0.5*dy;

    // Slope intersection testing
    // There are 10 ways a line can intersect a square -- how to determine which way?
    bool intersects = false;
    double y_xm_intersect;
    double y_xp_intersect;
    double test_ym;
    double test_yp;
    double x_slope_l, x_slope_r, y_slope_l, y_slope_r;
    // Test left
     = m*xmin+c;
    if( y_slope_l < ymax && y_slope_l > ymin){
        intersects = true;
        x_slope_l = xmin;
    }

    // Test right
    y_slope_r = m*xmax+c;
    if( y_slope_r < ymax && y_slope_l

    // Does the slope intersect this cell? If not, set standard boundary geometry
    
#else
    (void)x;
    (void)y;
    (void)m;
    (void)c;
#endif
    return;
}

}// Namespace Closure
#endif /* INITIALISERSLOPE_HPP */
