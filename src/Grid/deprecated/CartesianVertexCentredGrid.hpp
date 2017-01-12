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

    // Get start of real grid, in dim s
    unsigned int start( unsigned int s) const {
        (void)s;
        return this->ghosts();
    }

    // Get end of real grid, in dim s
    unsigned int end( unsigned int s) const {
        // Extra element in dimension s, unless s is the edge dimension
        return this->ghosts() + this->num_cells(s) + 1;
    }

    // Get physical position of location
    double position( unsigned int s, unsigned int ii) const {
        // ii should be turned to signed int to prevent integer underflow
        int ii_corrected = (int)ii-(int)(this->ghosts());
        return this->ds(s) * ( ii_corrected ); 
    }

    // Get total elements
    unsigned int total_elements() const {
        unsigned int total = 1;
        for( unsigned int s=0; s<dim; s++){
            total *= this->size(s)+1;
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
