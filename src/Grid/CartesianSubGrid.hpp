// CartesianSubGrid.hpp
//
// Inherits BaseCartesianGrid
// Defines indexing and total number of elements.
// Uses CartesianGridPolicies to constraint behaviour.

#ifndef CARTESIANSUBGRID_HPP
#define CARTESIANSUBGRID_HPP

#include "CartesianGrid.hpp"
#include "CartesianSubGridPolicies.hpp"

#include <type_traits>

namespace FVlite{

template<unsigned int dim, class HighSymmetryPolicy, class GhostPolicy>
class CartesianSubGrid : public virtual BaseCartesianGrid<dim>
    {

    // Determine at compile time if a legitimate GhostPolicy has been entered
    static_assert(std::is_base_of<BaseGhostPolicy,GhostPolicy>::value,"GridGhostPolicyError");

    // Determine whether legitimate HighSymmetryPolicy has been entered
    static_assert(std::is_base_of<BaseHSPolicy,HighSymmetryPolicy>::value,"GridHSPolicyError");

public:

    // Get number of ghost cells. Depends on GhostPolicy
    unsigned int ghosts() const{
        // Multiply buffer_size() by 1 if WithGhosts, by 0 otherwise 
        return this->buffer_size() * std::is_same<GhostPolicy,WithGhosts>::value;
    }

    double position( unsigned int s, unsigned int ii) const {
        // ii should be turned to signed int to prevent integer underflow
        return ((int)ii - (int)ghosts() + HighSymmetryPolicy::is_position_offset(s)*0.5) * this->ds(s);
    }

    // Number of real elements in direction s. Ignores buffer regions.
    // Uses the num_elements_offset(s) function from HighSymmetryPolicy.
    // This returns the extra number of data points in the grid relative
    // to the number of cells, i.e. for VertexCentred it is +1.
    unsigned int num_elements( unsigned int s) const {
        return this->num_cells(s) + HighSymmetryPolicy::is_extra_element(s);
    }

    // Get number of cells plus those in buffer regions, in direction s
    unsigned int full_width(unsigned int s) const {
        return this->num_cells(s) + 2*ghosts();
    }

    // Get number of elements plus those in buffer regions, in direction s
    unsigned int full_elements(unsigned int s) const {
        return num_elements(s) + 2*ghosts();
    }

    // Get first element of real grid, in dim s
    unsigned int start( unsigned int s) const {
        (void)s;
        return ghosts();
    }

    // Get end of real grid, in dim s
    unsigned int end( unsigned int s) const {
        return ghosts() + num_elements(s);
    }

    // Get total number of cells
    unsigned int total_cells() const {
        unsigned int total = 1;
        for( unsigned int s=0; s<dim; s++){
            total *= full_width(s);
        }
        return total;
    }

    // Get total number of elements
    unsigned int total_elements() const {
        unsigned int total = 1;
        for( unsigned int s=0; s<dim; s++){
            total *= full_elements(s);
        }
        return total;
    }

    // Generate access token from 3D indexing
    unsigned int get_idx( unsigned int ii,
                          unsigned int jj=0,
                          unsigned int kk=0 ) const {
    // Let compiler do the work here: only one if statement will make it to the compiled code
        if(dim==1){
            (void)jj;
            (void)kk;
            return ii;
        }
        if(dim==2){
            (void)kk;
            return ii + full_elements(DIM_X)*jj;
        }
        if(dim==3){
            return ii + full_elements(DIM_X)*(jj + full_elements(DIM_Y)*kk);
        }
    }


};


}
#endif
