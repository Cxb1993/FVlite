// LevelSetGridCartesianCentred
//
// Stores flux vectors for finite volume methods.

#ifndef LEVELSETGRID_HPP
#define LEVELSETGRID_HPP

#include <cmath>
#include <vector>

namespace FVlite{

template<class GridType>
class LevelSetGrid : public virtual GridType {
    protected:
        double min_level_set;
        std::vector<double> mLevelSet;
        std::vector<double> mWorkspace;
    public:
        LevelSetGrid() 
        {
            double lengths_squared = 0.0;
            for (unsigned int s=0; s<this->get_dim(); s++){
                lengths_squared += this->length(s) * this->length(s);
            }
            min_level_set = -1.0 * sqrt(lengths_squared);
            mLevelSet.resize( this->total_elements(), /*val=*/min_level_set);
            mWorkspace.resize( this->total_elements(), /*val=*/min_level_set);
        }

        double& levelset( unsigned int ii, unsigned int jj=0, unsigned int kk=0) {
            return mLevelSet[this->get_idx(ii,jj,kk)];
        }

        double levelset( unsigned int ii, unsigned int jj=0, unsigned int kk=0) const {
            return mLevelSet[this->get_idx(ii,jj,kk)];
        }

        double& workspace( unsigned int ii, unsigned int jj=0, unsigned int kk=0) {
            return mWorkspace[this->get_idx(ii,jj,kk)];
        }

        double workspace( unsigned int ii, unsigned int jj=0, unsigned int kk=0) const {
            return mWorkspace[this->get_idx(ii,jj,kk)];
        }

        unsigned int levelset_start( unsigned int dim){ return this->start(dim);}
        unsigned int levelset_end( unsigned int dim){ return this->end(dim);}
        double levelset_position( unsigned int dim, unsigned int ii){ return this->position(dim,ii);}


        void reset_levelset(){
            mLevelSet.assign( this->total_elements(), min_level_set);
        }

        void reset_workspace(){
            mWorkspace.assign( this->total_elements(), min_level_set);
        }

        void merge_levelset(){
            for( unsigned int ii=0; ii < this->total_elements(); ii++){
                mLevelSet[ii] = fmax( mLevelSet[ii], mWorkspace[ii]);
            }
        }

        // WARNING: THIS IS NOT CORRECT!    
        void intersect_levelset(){
            for( unsigned int ii=0; ii < this->total_elements(); ii++){
                mLevelSet[ii] = fmin( mLevelSet[ii], mWorkspace[ii]);
            }
        }

        double interpolate( double x, double y=0.0, double z=0.0);
};

// TODO 1D and 3D versions
template<>
double LevelSetGrid<CartesianSubGrid<2,CellCentred,WithGhosts>>::interpolate( double x, double y, double z){
// Function written some time ago and copied over with minor edits.
// May be bugged.
    (void)z;

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
