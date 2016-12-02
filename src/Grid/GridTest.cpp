// GridTest.cpp
//
// A simple program designed to test Grid.hpp

#define MAXWELL 1

#include <cstdlib>
#include <iostream>
#include "GridImplementation.hpp"
#include "StateGridCartesianCentred.hpp"
#include "FluxGridCartesianCentred.hpp"
#include "LevelSetGridCartesianCentred.hpp"
#include "CutCellGrids.hpp"

using namespace FVlite;

// Define grid

#define MY_DIM 2

typedef CartesianGridImplementation<
    MY_DIM,
    StateGridCartesianCentred<MY_DIM>,
    FluxGridCartesianCentred<MY_DIM>,
    LevelSetGridCartesianCentred<MY_DIM>,
    CutCellBoundaryStateGrid<MY_DIM>,
    CutCellGeometryGrid<MY_DIM>
> Grid;

// Test it out

int main(void){

    double L[2] = { 10.0, 10.0};
    unsigned int N[2] = {10,10};

    Grid grid( L, N, 1);

    StateVector vec1(1.0,1.0,1.0,1.0,1.0,1.0);    
    StateVector vec2(1.0,0.0,0.0,1.0,0.0,0.0);    
    grid.state(3,4) = vec1;
    FluxVector flux;
    flux.set(vec1,'x');
    grid.flux(1,5) = flux;

    grid.state(5,2) = vec2;
    StateVector vec3;
    vec3 = grid.state(3,4) + grid.state(5,2);
    std::cout << vec3[0] << ' '
              << vec3[1] << ' '
              << vec3[2] << ' '
              << vec3[3] << ' '
              << vec3[4] << ' '
              << vec3[5] << std::endl;

    std::cout << grid.position(DIM_X,5) << std::endl;
    std::cout << grid.position(DIM_Y,5) << std::endl;

    return EXIT_SUCCESS;
}
