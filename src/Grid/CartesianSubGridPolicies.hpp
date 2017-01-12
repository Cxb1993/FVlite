// CartesianSubGridPolicies.hpp
//
// A collection of policy classes used to modify Cartesian grid behaviour.
//
// > HighSymmetryPolicy
//   - Specifies where data is stored on the grid.
//   - FaceCentred, EdgeCentred, CellCentred, VertexCentred.
//   - Affects grid size, indexing, and start() and end() functions.
// > GhostsPolicy
//   - Controls whether SubGrid contains a buffer region of 'ghost cells'
//     around the actual domain.
//   - WithGhosts and NoGhosts
//   - Defaults chosen in many cases.
//   - The shared base grid specifies the number of ghost cells, and using
//     this policy, subgrids can opt in or out of using them.
//   - Removing ghosts allows for easier looping over the grid, and saves
//     memory.
//   - Note that if one subgrid has ghosts but the other doesn't, they will
//     have different sizes and will require independent indexing.
//   - Affects start() and end() functions

#ifndef CARTESIANSUBGRIDPOLICIES_HPP
#define CARTESIANSUBGRIDPOLICIES_HPP

namespace FVlite{

// Ghost policies

class BaseGhostPolicy {};
class WithGhosts : public BaseGhostPolicy{};
class NoGhosts : public BaseGhostPolicy{};

// High symmetry policies

class BaseHSPolicy {};

class CellCentred : public BaseHSPolicy {
public:
    // Returns true if extra data point required
    // relative to the number of cells. i.e.
    // vertex centred data always requires an
    // extra element, cell centred doesn't.
    static bool is_extra_element( unsigned int s){
       (void)s;
       return 0;
    }

    // Returns true if physical position is offset
    // by 0.5*cell_spacing relative to lower left
    // vertex
    static bool is_position_offset( unsigned int s){
        (void)s;
        return 1;
    }
};

class VertexCentred : public BaseHSPolicy {
public:
    static bool is_extra_element( unsigned int s){
       (void)s;
       return 1;
    }
    static bool is_position_offset( unsigned int s){
        (void)s;
        return 0;
    }
};

// Face and edge centred are also templated over the
// direction of the face or edge under consideration.
// edge_dim==DIM_X refers to x-pointing edges
// face_dim==DIM_X refers to faces in the yz plane (x is the normal)

template<unsigned int face_dim>
class FaceCentred : public BaseHSPolicy {
public:
    static bool is_extra_element( unsigned int s){
        return (s==face_dim);
    }
    static bool is_position_offset( unsigned int s){
        return (s!=face_dim);
    }
};

template<unsigned int edge_dim>
class EdgeCentred : public BaseHSPolicy {
public:
    static bool is_extra_element( unsigned int s){
        return (s!=edge_dim);
    }
    static bool is_position_offset( unsigned int s){
        return (s==edge_dim);
    }
};

}
#endif
