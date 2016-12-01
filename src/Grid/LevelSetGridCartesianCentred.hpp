// LevelSetGridCartesianCentred
//
// Stores flux vectors for finite volume methods.

#ifndef LEVELSETGRIDCARTESIANCENTRED_HPP
#define LEVELSETGRIDCARTESIANCENTRED_HPP

#include "BaseGridCartesianCentred.hpp"
#include <vector>

namespace FVlite{

template<unsigned int dim>
class LevelSetGridCartesianCentred : public virtual BaseGridCartesianCentred<dim> {
    protected:
        std::vector<double> mLevelSet;
    public:
        LevelSetGridCartesianCentred() 
        {
            mLevelSet.resize( this->total_cells(), /*val=*/0.0);
        }

        double& levelset( unsigned int ii, unsigned int jj=0, unsigned int kk=0) {
            return mLevelSet[this->get_idx(ii,jj,kk)];
        }

        double levelset( unsigned int ii, unsigned int jj=0, unsigned int kk=0) const {
            return mLevelSet[this->get_idx(ii,jj,kk)];
        }

        LevelSetGridCartesianCentred get_levelset_workspace(){
            LevelSetGridCartesianCentred workspace( this->mLs, this->mNs, this->mGhosts);
            return workspace;
        }

        void reset_levelset(){
            mLevelSet.assign( this->total_cells(), 0.0);
        }

        void merge( LevelSetGridCartesianCentred& other){
            for( int ii=0; ii<this->total_cells(); ii++){
                mLevelSet[ii] = fmax( mLevelSet[ii], other.mLevelSet[ii]);
            }
        }

        // WARNING: THIS IS NOT CORRECT!    
        void intersect( LevelSetGridCartesianCentred& other){
            for( int ii=0; ii<this->total_cells(); ii++){
                mLevelSet[ii] = fmin( mLevelSet[ii], other.mLevelSet[ii]);
            }
        }

        double interpolate( double x, double y=0.0, double z=0.0);
};

// TODO 1D and 3D versions
template<>
double LevelSetGridCartesianCentred<2>::interpolate( double x, double y, double z){
// Function written some time ago and copied over with minor edits.
// May be bugged.

    // Performs bilinear interpolation to determine level set function
    // at arbitrary location.
    // Something to note:
    // Level set function defined at cell centers: half-integer locations.
    // Must keep this in mind at all times.

    int bound = this->ghosts();
    int Nx = this->num_cells(DIM_X);
    int Ny = this->num_cells(DIM_Y);
    double dx = this->ds(DIM_X);
    double dy = this->ds(DIM_Y);
    double Lx = this->length(DIM_X);
    double Ly = this->length(DIM_Y);

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
    double ls_lower_lower = this->levelset(ii_lower,jj_lower);
    double ls_upper_lower = this->levelset(ii_upper,jj_lower);
    double ls_lower_upper = this->levelset(ii_lower,jj_upper);
    double ls_upper_upper = this->levelset(ii_upper,jj_upper);

    // Bilinear interpolation -- algorithm from wikipedia page
    double f_y1 = ((x_upper-x)*ls_lower_lower+(x-x_lower)*ls_upper_lower)/(x_upper-x_lower);
    double f_y2 = ((x_upper-x)*ls_lower_upper+(x-x_lower)*ls_upper_upper)/(x_upper-x_lower);
    double f    = ((y_upper-y)*f_y1+(y-y_lower)*f_y2)/(y_upper-y_lower);

    if( fabs(f) < dx*1e-4) return dx*1e-4;
    return f;
}
    

}
#endif
