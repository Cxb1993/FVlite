// GridTest.cpp
//
// A simple program designed to test Grids

#define MAXWELL 1
#define MY_DIM 2

#include <cstdlib>
#include <iostream>
#include "CartesianSubGrid.hpp"
#include "SubGrids/StateGrid.hpp"
#include "SubGrids/FluxGrid.hpp"
#include "SubGrids/MaterialGrid.hpp"

using namespace FVlite;

// Define grid

typedef CartesianSubGrid<MY_DIM,CellCentred,WithGhosts> PrimaryGrid;
typedef CartesianSubGrid<MY_DIM,EdgeCentred<DIM_X>,NoGhosts> SecondaryGrid;

typedef CartesianGrid<
    MY_DIM,
    StateGrid<PrimaryGrid>,
    MaterialGrid<Material,PrimaryGrid>,
    FluxGrid<SecondaryGrid>
> Grid;

// Test it out

int main(void){

    double L[2] = { 10.0, 5.0};
    unsigned int N[2] = {10,5};

    Grid grid( L, N, 2);

    Material mat(1.0,1.0);
    StateVector vec1(1.0,1.0,1.0,1.0,1.0,1.0);    
    StateVector vec2(1.0,0.0,0.0,1.0,0.0,0.0);    
    grid.state(3,4) = vec1;
    FluxVector flux;
    flux.set(vec1,mat,'x');
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

    std::cout << "StateGrid:" << std::endl;
    for( int jj=0; jj<grid.PrimaryGrid::full_elements(DIM_Y); jj++){
        for( int ii=0; ii<grid.PrimaryGrid::full_elements(DIM_X); ii++){
            std::cout << "(" ;
            std::cout <<  grid.PrimaryGrid::position(DIM_X,ii) << ",";
            std::cout <<  grid.PrimaryGrid::position(DIM_Y,jj) << ") ";
        }
        std::cout << std::endl;
    }

    std::cout << "FluxGrid:" << std::endl;
    for( int jj=0; jj<grid.SecondaryGrid::full_elements(DIM_Y); jj++){
        for( int ii=0; ii<grid.SecondaryGrid::full_elements(DIM_X); ii++){
            std::cout << "(" ;
            std::cout <<  grid.SecondaryGrid::position(DIM_X,ii) << ",";
            std::cout <<  grid.SecondaryGrid::position(DIM_Y,jj) << ") ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nUsing start/end:" << std::endl;
    std::cout << "StateGrid:" << std::endl;
    for( int jj=grid.PrimaryGrid::start(DIM_Y); jj<grid.PrimaryGrid::end(DIM_Y); jj++){
        for( int ii=grid.PrimaryGrid::start(DIM_X); ii<grid.PrimaryGrid::end(DIM_X); ii++){
            std::cout << "(" ;
            std::cout <<  grid.PrimaryGrid::position(DIM_X,ii) << ",";
            std::cout <<  grid.PrimaryGrid::position(DIM_Y,jj) << ") ";
        }
        std::cout << std::endl;
    }

    std::cout << "FluxGrid:" << std::endl;
    for( int jj=grid.SecondaryGrid::start(DIM_Y); jj<grid.SecondaryGrid::end(DIM_Y); jj++){
        for( int ii=grid.SecondaryGrid::start(DIM_X); ii<grid.SecondaryGrid::end(DIM_X); ii++){
            std::cout << "(" ;
            std::cout <<  grid.SecondaryGrid::position(DIM_X,ii) << ",";
            std::cout <<  grid.SecondaryGrid::position(DIM_Y,jj) << ") ";
        }
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
