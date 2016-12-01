// Grid.hpp
//
// Defines grid to be used.
// Should be replaced in future with a more versatile implementation which
// can be easily configured at compile time.

#ifndef GRIDTYPEDEF_HPP
#define GRIDTYPEDEF_HPP

#include "GridImplementation.hpp"
#include "StateGridCartesianCentred.hpp"
#include "FluxGridCartesianCentred.hpp"
#include "LevelSetGridCartesianCentred.hpp"
#include "CutCellGrids.hpp"

namespace FVlite{

#define MY_DIM 2

typedef CartesianGridImplementation<
    MY_DIM,
    StateGridCartesianCentred<MY_DIM>,
    FluxGridCartesianCentred<MY_DIM>,
    LevelSetGridCartesianCentred<MY_DIM>,
    CutCellBoundaryStateGrid<MY_DIM>,
    CutCellGeometryGrid<MY_DIM>
> Grid;

}
#endif
