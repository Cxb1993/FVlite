// CartesianEdgeCentredGrid.hpp
//
// Inherits BaseCartesianGrid
// Defines indexing and total number of elements.
// 
// Stores along one edge of a Cartesian cell, specified
// by the template parameter edge_dim. For example,
// if edge_dim==DIM_X, the first element is located at
// position (0.5*dx,0,0). An extra element would be stored
// in the directions which aren't DIM_X, so the dimensions
// of this grid are actually ( Nx, Ny+1, Nz+1)

#ifndef CARTESIANEDGECENTREDGRID_HPP
#define CARTESIANEDGECENTREDGRID_HPP

namespace FVlite{

template<unsigned int dim, unsigned int edge_dim>
class CartesianEdgeCentredGrid : public virtual BaseCartesianGrid<dim> {

    public:

    // Get total elements
    unsigned int total_elements() const {
        unsigned int total = 1;
        for( unsigned int s=0; s<dim; s++){
            total *= ( s==edge_dim ) ? size(s) : size(s)+1;
        }
        return total;
    }

    // Get 1D array index equivalent to 3D indexing of (i,j,k)
    unsigned int get_idx( unsigned int ii, unsigned int jj=0, unsigned int kk=0) const;

};

template<unsigned int edge_dim>
unsigned int CartesianEdgeCentredGrid<1,edge_dim>::get_idx( unsigned int ii, unsigned int jj, unsigned int kk) const {
    (void)jj;
    (void)kk;
    return ii;
}

template<unsigned int edge_dim>
unsigned int CartesianEdgeCentredGrid<2,edge_dim>::get_idx( unsigned int ii, unsigned int jj, unsigned int kk) const{
    (void)kk;
    // number of elements in x direction is size(DIM_X)+1, unless on x-directed edges, in which
    // case there are size(DIM_X)
    unsigned int x_size = size(DIM_X) + (edge_dim != DIM_X);
    return ii + x_size*jj;
}

template<unsigned int edge_dim>
unsigned int CartesianEdgeCentredGrid<3,edge_dim>::get_idx( unsigned int ii, unsigned int jj, unsigned int kk) const{
    // number of elements in x direction is size(DIM_X)+1, unless on x-directed edges, in which
    // case there are size(DIM_X)
    unsigned int x_size = size(DIM_X) + (edge_dim != DIM_X);
    unsigned int y_size = size(DIM_Y) + (edge_dim != DIM_Y);
    return ii + x_size*(jj + y_size*kk);
}

}
#endif
