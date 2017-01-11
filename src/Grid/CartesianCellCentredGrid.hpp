// CartesianCellCentredGrid.hpp
//
// Inherits BaseCartesianGrid
// Defines indexing and total number of elements. (In this case, same as number of cells)

#ifndef CARTESIANCELLCENTREDGRID_HPP
#define CARTESIANCELLCENTREDGRID_HPP

namespace FVlite{

template<unsigned int dim>
class CartesianCellCentredGrid : public virtual BaseCartesianGrid<dim> {
public:

    // Get start of real grid, in dim s
    unsigned int start( unsigned int s) const {
        (void)s;
        return this->ghosts();
    }

    // Get end of real grid, in dim s
    unsigned int end( unsigned int s) const {
        return this->ghosts() + this->num_cells(s);
    }

    // Get physical position of location
    double position( unsigned int s, unsigned int ii) const {
        // ii should be turned to signed int to prevent integer underflow
        return ((int)ii-(int)(this->ghosts())+0.5)*this->ds(s);
    }

    // Get total elements
    unsigned int total_elements() const {
        return this->total_cells();
    }

    // Get 1D array index equivalent to 3D indexing of (i,j,k)
    unsigned int get_idx( unsigned int ii, unsigned int jj=0, unsigned int kk=0) const;

};

template<>
unsigned int CartesianCellCentredGrid<1>::get_idx( unsigned int ii, unsigned int jj, unsigned int kk) const {
    (void)jj;
    (void)kk;
    return ii;
}

template<>
unsigned int CartesianCellCentredGrid<2>::get_idx( unsigned int ii, unsigned int jj, unsigned int kk) const{
    (void)kk;
    return ii + size(DIM_X)*jj;
}

template<>
unsigned int CartesianCellCentredGrid<3>::get_idx( unsigned int ii, unsigned int jj, unsigned int kk) const{
    return ii + size(DIM_X)*(jj + size(DIM_Y)*kk);
}

}
#endif
