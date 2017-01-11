// CartesianGrid
// 
// Contains base class for Cartesian grids, with information stored at cell centres.
// Used extensively for Cartesian Finite Volume implementations.
//
// Also contains generic Cartesian grid class.

#ifndef BASECARTESIANGRID_HPP
#define BASECARTESIANGRID_HPP

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

// Base Class

template<unsigned int dim>
class BaseCartesianGrid : public BaseGrid {
protected:
    unsigned int mSizes[dim]; // Number of cells spanning each direction. Includes ghost cells.
    double mLs[dim];          // Physical length of grid in each direction
    double mDs[dim];          // Grid spacing, L/N
    unsigned int mGhosts;     // Number of ghost cells. Uniform across directions.
public:
    BaseCartesianGrid(){}
    BaseCartesianGrid( double Ls[dim], unsigned int Ns[dim], unsigned int ghosts = 0) { 
        mGhosts = ghosts;
        for(unsigned int s=0; s<dim; s++){
            mSizes[s] = Ns[s] + 2*mGhosts;
            mLs[s] = Ls[s];
            mDs[s] = Ls[s]/Ns[s];
        }
    }

    virtual ~BaseCartesianGrid(){}

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

    // Get physical position of location
    double position( unsigned int s, unsigned int ii) const {
        // ii should be turned to signed int to prevent integer underflow
        return ((int)ii-(int)ghosts()+0.5)*ds(s);
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

};

// Generic Class

template<unsigned int dim, typename... Subgrids>
class CartesianGrid :
    public Subgrids...
{
    public:
    // TODO generalise this. Perhaps demand a Builder, or a clever Factory?
    CartesianGrid( double Ls[dim], unsigned int Ns[dim], unsigned int ghosts) :
        // Build base grid first. Subgrids require its parameters to construct themselves.
        BaseCartesianGrid<dim>(Ls,Ns,ghosts),
        // Construct all subgrids specified in template params
        Subgrids()...
    {}
};

}
#endif
