// LevelSet.hpp
//
// Controls level set functions for boundary region generation.

#ifndef LEVELSET_HPP
#define LEVELSET_HPP

#include <cmath>
#include <vector>

#include "Vector3.hpp"
#include "Grid.hpp"

namespace FVTD{

class LevelSet{

private:

    // Grid properties. Copies stored here to avoid Grid lookups.
    int sizeX, sizeY; // Grid sizes
    int bound;        // Number of boundary cells
    double dx, dy;    // Grid spacing
    double Lx, Ly;    // Physical domain sizes

    // Containers for the level set function
    // mValues contains the level set function itself
    // mWorkspace is a grid of the same dimensions.
    // All drawing should be performed on mWorkspace. To edit the level set
    // function, draw to mWorkspace and merge/intersect.
    std::vector<double> mValues;
    std::vector<double> mWorkspace;

public:

    LevelSet( int Nx, int Ny, int bound, double dx, double dy, double Lx, double Ly);
    ~LevelSet();

    // Init sets mValues to background
    void init();

    // Merge/intersect workspace with the level set
    // Both act as a sort of 'commit changes' command
    void merge();
    void intersect();

    // Parenthesis access functions. Behaves as get/set for level set
    double operator() ( int ii, int jj) const;
    double& operator() ( int ii, int jj);

    // Performs bilinear interpolation to obtain level set at arbitrary location
    double interpolate( double x, double y) const;

    // Access functions for the 'workspace'
    double workspace( int ii, int jj) const;
    double& workspace( int ii, int jj);

};



LevelSet::LevelSet( int Nx, int Ny, int bound, double dx, double dy, double Lx, double Ly) : 
        sizeX(Nx+2*bound), sizeY(Ny+2*bound), bound(bound),
        dx(dx), dy(dy), Lx(Lx), Ly(Ly),
        mValues(sizeX*sizeY), mWorkspace(sizeX*sizeY){
}

LevelSet::~LevelSet(){}

void LevelSet::init(){
    double background = -1.*sqrt(Lx*Lx+Ly*Ly);
    for( int jj=0; jj<sizeY; jj++){
        for( int ii=0; ii<sizeX;ii++){
            (*this)(ii,jj) = background;
        }
    }
    return;
}

void LevelSet::merge(){
    for( int jj=0; jj<sizeY; jj++){
       for( int ii=0; ii<sizeX; ii++){
           (*this)(ii,jj) = fmax( (*this)(ii,jj), workspace(ii,jj));
        }
    }
}

void LevelSet::intersect(){
    for( int jj=0; jj<sizeY; jj++){
       for( int ii=0; ii<sizeX; ii++){
           (*this)(ii,jj) = fmin( (*this)(ii,jj), workspace(ii,jj));
        }
    }
}

double LevelSet::operator()( int ii, int jj) const{
    double result = mValues[jj*sizeX+ii];
    if( fabs(result) < dx*1e-4 ) return dx*1e-4; 
    return mValues[jj*sizeX+ii];
}

double& LevelSet::operator()( int ii, int jj){
    return mValues[jj*sizeX+ii];
}

double LevelSet::interpolate( double x, double y) const{
    // Performs bilinear interpolation to determine level set function
    // at arbitrary location.
    // Something to note:
    // Level set function defined at cell centers: half-integer locations.
    // Must keep this in mind at all times.

    int Nx = sizeX - 2*bound;
    int Ny = sizeY - 2*bound;

    // subtract 0.5dx from x, 0.5dy from y.
    // This is done because (ii,jj) describes cell vertices, not cell centers
    double x_vertex = x - 0.5*dx;
    double y_vertex = y - 0.5*dy;

    // Find cell numbers for all four cell centers surrounding (x,y)
    int ii_lower = floor(Nx*x_vertex/Lx) + bound;
    int ii_upper = ceil(Nx*x_vertex/Lx) + bound;
    int jj_lower = floor(Ny*y_vertex/Ly) + bound;
    int jj_upper = ceil(Ny*y_vertex/Ly) + bound;

    // Find coordinates of cell centers surrounding (x,y)
    double x_lower = (ii_lower-bound+0.5)*dx;
    double x_upper = (ii_upper-bound+0.5)*dx;
    double y_lower = (jj_lower-bound+0.5)*dy;
    double y_upper = (jj_upper-bound+0.5)*dy;

    // Get level set at each of these corners -- TODO extrapolate if needed TODO
    double ls_lower_lower = (*this)(ii_lower,jj_lower);
    double ls_upper_lower = (*this)(ii_upper,jj_lower);
    double ls_lower_upper = (*this)(ii_lower,jj_upper);
    double ls_upper_upper = (*this)(ii_upper,jj_upper);

    // Bilinear interpolation -- algorithm from wikipedia page
    double f_y1 = ((x_upper-x)*ls_lower_lower+(x-x_lower)*ls_upper_lower)/(x_upper-x_lower);
    double f_y2 = ((x_upper-x)*ls_lower_upper+(x-x_lower)*ls_upper_upper)/(x_upper-x_lower);
    double f    = ((y_upper-y)*f_y1+(y-y_lower)*f_y2)/(y_upper-y_lower);

    if( fabs(f) < dx*1e-4) return dx*1e-4;
    return f;
}

double LevelSet::workspace( int ii, int jj) const{
    return mWorkspace[jj*sizeX+ii];
}

double& LevelSet::workspace( int ii, int jj){
    return mWorkspace[jj*sizeX+ii];
}

}// Namespace closure
#endif /* LEVELSET_HPP */
