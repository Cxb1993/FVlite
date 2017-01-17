// Grid.hpp
//
// Defines grid to be used.
// Should be replaced in future with a more versatile implementation which
// can be easily configured at compile time.

#ifndef GRIDTYPEDEF_HPP
#define GRIDTYPEDEF_HPP

#include "CartesianGrid.hpp"
#include "ConstantGrid.hpp"

#include "SubGrids/StateGrid.hpp"
#include "SubGrids/FluxGrid.hpp"
#include "SubGrids/LevelSetGrid.hpp"
#include "SubGrids/MaterialGrid.hpp"
#include "SubGrids/CutCellGrids.hpp"

namespace FVlite{

// TODO allow this to be changed using compiler options

#define MY_DIM 2

typedef CartesianGrid<
    MY_DIM,
    StateGrid< CartesianCellCentredGrid<MY_DIM> >,
    FluxGrid< CartesianCellCentredGrid<MY_DIM> >,
#ifdef MAXWELL
    MaterialGrid< Material, CartesianCellCentredGrid<MY_DIM> >,
#endif
#ifdef EULER
    MaterialGrid< Material, ConstantGrid>,
#endif
    LevelSetGrid<MY_DIM>,
    CutCellBoundaryStateGrid< CartesianCellCentredGrid<MY_DIM> >,
    CutCellGeometryGrid< CartesianCellCentredGrid<MY_DIM> >
> Grid;

}
#endif
