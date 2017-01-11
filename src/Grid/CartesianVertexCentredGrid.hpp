// CartesianVertexCentredGrid.hpp
//
// Inherits BaseCartesianGrid
// Defines indexing and total number of elements.
// 
// Stores at vertices of a Cartesian cell.
// Note that there is always one more vertex per dimension
// than there are cells.

#ifndef CARTESIANVERTEXCENTREDGRID_HPP
#define CARTESIANVERTEXCENTREDGRID_HPP

namespace FVlite{

template<unsigned int dim>
class CartesianVertexCentredGrid : public virtual BaseCartesianGrid<dim> {

    public:

    // Get total elements
    unsigned int total_elements() const {
        unsigned int total = 1;
        for( unsigned int s=0; s<dim; s++){
            total *= size(s)+1;
        }
        return total;
    }

    // Get 1D array index equivalent to 3D indexing of (i,j,k)
    unsigned int get_idx( unsigned int ii, unsigned int jj=0, unsigned int kk=0) const;

};

template<>
unsigned int CartesianVertexCentredGrid<1>::get_idx( unsigned int ii, unsigned int jj, unsigned int kk) const {
    (void)jj;
    (void)kk;
    return ii;
}

template<>
unsigned int CartesianVertexCentredGrid<2>::get_idx( unsigned int ii, unsigned int jj, unsigned int kk) const{
    (void)kk;
    unsigned int x_size = size(DIM_X) + 1;
    return ii + x_size*jj;
}

template<>
unsigned int CartesianVertexCentredGrid<3>::get_idx( unsigned int ii, unsigned int jj, unsigned int kk) const{
    unsigned int x_size = size(DIM_X) + 1;
    unsigned int y_size = size(DIM_Y) + 1;
    return ii + x_size*(jj + y_size*kk);
}

}
#endif
