// Grid.hpp
//
// Defines grid to be used.
// Should be replaced in future with a more versatile implementation which
// can be easily configured at compile time.

#ifndef GRIDTYPEDEF_HPP
#define GRIDTYPEDEF_HPP

// TODO allow this to be changed using compiler options
#define DIM 2

#include "CartesianSubGrid.hpp"
#include "ConstantSubGrid.hpp"

#include "SubGrids/StateGrid.hpp"
#include "SubGrids/FluxGrid.hpp"
#include "SubGrids/LevelSetGrid.hpp"
#include "SubGrids/MaterialGrid.hpp"
#include "SubGrids/CutCellGrids.hpp"

namespace FVlite{

typedef CartesianGrid<
    DIM,
    StateGrid< CartesianSubGrid< DIM, CellCentred, WithGhosts> >,
    FluxGrid< CartesianSubGrid< DIM, CellCentred, WithGhosts> >,
#ifdef MAXWELL
    MaterialGrid< Material, CartesianSubGrid<DIM,CellCentred, WithGhosts> >,
#endif
#ifdef EULER
    MaterialGrid< Material, ConstantGrid>,
#endif
    LevelSetGrid<DIM>,
    CutCellBoundaryStateGrid< CartesianSubGrid< DIM, CellCentred, WithGhosts> >,
    CutCellGeometryGrid< CartesianSubGrid< DIM, CellCentred, WithGhosts> >
> Grid;

}
#endif
