// Grid.hpp
//
// Defines grid to be used.
// Should be replaced in future with a more versatile implementation which
// can be easily configured at compile time.

#ifndef GRIDTYPEDEF_HPP
#define GRIDTYPEDEF_HPP

// TODO allow this to be changed using compiler options
#define MY_DIM 2

#include "CartesianSubGrid.hpp"
#include "ConstantGrid.hpp"

#include "SubGrids/StateGrid.hpp"
#include "SubGrids/FluxGrid.hpp"
#include "SubGrids/LevelSetGrid.hpp"
#include "SubGrids/MaterialGrid.hpp"
#include "SubGrids/CutCellGrids.hpp"

namespace FVlite{

typedef CartesianGrid<
    MY_DIM,
    StateGrid< CartesianSubGrid<MY_DIM, CellCentred, WithGhosts> >,
    FluxGrid< CartesianSubGrid<MY_DIM, CellCentred, WithGhosts> >,
#ifdef MAXWELL
    MaterialGrid< Material, CartesianSubGrid<MY_DIM, CellCentred, WithGhosts> >,
#endif
#ifdef EULER
    MaterialGrid< Material, ConstantGrid>,
#endif
    LevelSetGrid<MY_DIM>,
    CutCellBoundaryStateGrid< CartesianSubGrid<MY_DIM, CellCentred, WithGhosts> >,
    CutCellGeometryGrid< CartesianSubGrid<MY_DIM, CellCentred, WithGhosts> >
> Grid;

}
#endif
