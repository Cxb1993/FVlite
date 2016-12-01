
#ifndef MYGRID
#define MYGRID

#include "GridImplementation.hpp"
#include "StateGridCartesianCentred.hpp"
#include "FluxGridCartesianCentred.hpp"

namespace FVlite {

// Implementation
// TODO allow this to be changes using compiler options

#define MY_DIM 2

typedef CartesianGridImplementation<
    MY_DIM,
    StateGridCartesianCentred<MY_DIM>,
    FluxGridCartesianCentred<MY_DIM>
> Grid;

}

#endif
