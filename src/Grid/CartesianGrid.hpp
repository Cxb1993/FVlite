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

    unsigned int mNumCells[dim]; // Number of cells spanning each direction.
    double mLengths[dim];        // Physical length of grid in each direction.
    double mSpacings[dim];       // Grid spacing, L/N.
    unsigned int mBufSize;       // Thickness of buffer region surrounding the grid.

public:

    BaseCartesianGrid(){}

    BaseCartesianGrid( double Ls[dim], unsigned int Ns[dim], unsigned int bufSize = 0) { 
        mBufSize = bufSize;
        for(unsigned int s=0; s<dim; s++){
            mNumCells[s] = Ns[s];
            mLengths[s] = Ls[s];
            mSpacings[s] = Ls[s]/Ns[s];
        }
    }

    ~BaseCartesianGrid(){}

    // Return dimensionality
    static unsigned int get_dim() {
        return dim;
    }

    // Get number of cells in real grid, in dim s 
    unsigned int num_cells( unsigned int s) const {
        return mNumCells[s];
    }

    // Get physical extent of real grid, in dim s
    double length( unsigned int s) const {
        return mLengths[s];
    }

    // Get grid spacing, in dim s
    double ds( unsigned int s) const {
        return mSpacings[s];
    }

    // Get width of buffer region
    unsigned int buffer_size() const {
        return mBufSize;
    }

    // Get minimum grid spacing
    double min_ds() const {
        double min = ds(0);
        for( unsigned int s=1; s<dim; s++){
            min = ( ds(0) < min ) ? ds(0) : min;
        }
        return min;
    }


};

// Generic Class

template<unsigned int dim, typename... Subgrids>
class CartesianGrid :
    public Subgrids...
{
    public:
    // TODO generalise this. Perhaps demand a Builder, or a clever Factory?
    CartesianGrid( double Ls[dim], unsigned int Ns[dim], unsigned int bufSize) :
        // Build base grid first. Subgrids require its parameters to construct themselves.
        BaseCartesianGrid<dim>(Ls,Ns,bufSize),
        // Construct all subgrids specified in template params
        Subgrids()...
    {}
};

}
#endif
