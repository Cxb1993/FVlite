// GridImplementation
//
// General Grid class. Builds Grids containing arbitrary contents.

#ifndef GRIDIMPL_HPP
#define GRIDIMPL_HPP

#include "BaseGridCartesianCentred.hpp"

namespace FVlite{

// General Grid

template<unsigned int dim, typename... Subgrids>
class CartesianGridImplementation :
    public Subgrids...
{
    public:
    // TODO generalise this. Perhaps demand a Builder, or a clever Factory?
    CartesianGridImplementation( double Ls[dim], unsigned int Ns[dim], unsigned int ghosts) :
        // Build base grid first. Subgrids require its parameters to construct themselves.
        BaseGridCartesianCentred<dim>(Ls,Ns,ghosts),
        // Construct all subgrids specified in template params
        Subgrids()...
    {}
};


}
#endif
