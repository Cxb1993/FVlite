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
#include "MaterialGridCartesianCentred.hpp"
#include "MaterialGridConstant.hpp"
#include "LevelSetGridCartesianCentred.hpp"
#include "CutCellGrids.hpp"

namespace FVlite{

// TODO allow this to be changed using compiler options

#define MY_DIM 2

typedef CartesianGridImplementation<
    MY_DIM,
    StateGridCartesianCentred<MY_DIM>,
    FluxGridCartesianCentred<MY_DIM>,
#ifdef MAXWELL
    MaterialGridCartesianCentred<MY_DIM,Material>,
#endif
#ifdef EULER
    MaterialGridConstant<Material>,
#endif
    LevelSetGridCartesianCentred<MY_DIM>,
    CutCellBoundaryStateGrid<MY_DIM>,
    CutCellGeometryGrid<MY_DIM>
> Grid;

}
#endif
