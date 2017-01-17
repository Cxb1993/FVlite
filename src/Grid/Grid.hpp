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
#include "SubGrids/Flux1DGrid.hpp"
#include "SubGrids/LevelSetGrid.hpp"
#include "SubGrids/MaterialGrid.hpp"
#include "SubGrids/CutCellGrids.hpp"

namespace FVlite{

typedef CartesianSubGrid<DIM,CellCentred,WithGhosts> MainGrid;

typedef CartesianGrid<
    DIM,
    StateGrid< MainGrid >,
    Flux1DGrid< DIM >,
#ifdef MAXWELL
    MaterialGrid< Material, MainGrid >,
#endif
#ifdef EULER
    MaterialGrid< Material, ConstantSubGrid>,
#endif
    CutCellBoundaryStateGrid< MainGrid >,
    CutCellGeometryGrid< MainGrid >,
    LevelSetGrid< CartesianSubGrid<DIM,VertexCentred,WithGhosts> >
> Grid;

}
#endif
