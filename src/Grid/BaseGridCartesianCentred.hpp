// BaseCartesianCentredGrid
//
// Base class for Cartesian grids, with information stored at cell centres.
// Used extensively for Cartesian Finite Volume implementations.

#ifndef BASEGRIDCARTESIANCENTRED_HPP
#define BASEGRIDCARTESIANCENTRED_HPP

// Define access macros
// These should be use to access data, but not to specify the maximum dimension!
// To specify you want 3 dimensions, BaseCartesianCentredGrid<3>.
// To find, for example, the number of cells in the z direction, you need:
// grid.n_s(2);
// Or alternatively, to improve readability:
// grid.n_s(DIM_Z);

#define DIM_X 0
#define DIM_Y 1
#define DIM_Z 2

#include "BaseGrid.hpp"

namespace FVlite{

template<unsigned int dim>
class BaseGridCartesianCentred : public BaseGrid {
protected:
    unsigned int mSizes[dim]; // Number of cells spanning each direction. Includes ghost cells.
    double mLs[dim];          // Physical length of grid in each direction
    double mDs[dim];          // Grid spacing, L/N
    unsigned int mGhosts;     // Number of ghost cells. Uniform across directions.
protected:
    BaseGridCartesianCentred(){}
    BaseGridCartesianCentred( double Ls[dim], unsigned int Ns[dim], unsigned int ghosts) { 
        mGhosts = ghosts;
        for(unsigned int s=0; s<dim; s++){
            mSizes[s] = Ns[s] + 2*mGhosts;
            mLs[s] = Ls[s];
            mDs[s] = Ls[s]/Ns[s];
        }
    }

    virtual ~BaseGridCartesianCentred(){}

    // Get number of cells in real grid, in dim s 
    unsigned int num_cells( unsigned int s) const {
        return mSizes[s] - 2*mGhosts;
    }

    // Get number of cells in grid, including ghost cells, in dim s
    unsigned int size( unsigned int s) const {
        return mSizes[s];
    }

    // Get start of real grid, in dim s
    unsigned int start( unsigned int s) const {
        (void)s;
        return mGhosts;
    }

    // Get end of real grid, in dim s
    unsigned int end( unsigned int s) const {
        return mGhosts + num_cells(s);
    }

    // Get physical extent of real grid, in dim s
    double length( unsigned int s) const {
        return mLs[s];
    }

    // Get grid spacing, in dim s
    double ds( unsigned int s) const {
        return mDs[s];
    }

    // Get minimum grid spacing
    double min_ds() const {
        double min = ds(0);
        for( unsigned int s=1; s<dim; s++){
            min = ( ds(0) < min ) ? ds(0) : min;
        }
        return min;
    }

    // Get number of 'ghost cells'
    unsigned int ghosts() const {
        return mGhosts;
    }

    // Get total number of cells in grid.
    unsigned int total_cells() const {
        double total = 1.0;
        for( unsigned int s=0; s<dim; s++){
            total *= size(s);
        }
        return total;
    }

    // Get 1D array index equivalent to 3D indexing of (i,j,k)
    unsigned int get_idx( unsigned int ii, unsigned int jj=0, unsigned int kk=0);

};

template<>
unsigned int BaseGridCartesianCentred<1>::get_idx( unsigned int ii, unsigned int jj, unsigned int kk){
    (void)jj;
    (void)kk;
    return ii;
}

template<>
unsigned int BaseGridCartesianCentred<2>::get_idx( unsigned int ii, unsigned int jj, unsigned int kk){
    (void)kk;
    return ii + size(DIM_X)*jj;
}

template<>
unsigned int BaseGridCartesianCentred<3>::get_idx( unsigned int ii, unsigned int jj, unsigned int kk){
    (void)kk;
    return ii + size(DIM_X)*(jj + size(DIM_Y)*kk);
}

}
#endif
