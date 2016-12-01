// FVlite.cpp
//
// Code to run FVlite.hpp.
// See the README for information on how to use this code.
// TODO Write dat README

#include <cstdlib>
#include <string>
#include <iostream>

#include <libconfig.h++>

#include "FVlite.hpp"

using namespace FVlite;
using std::string;

int main( int argc, char* argv[]){

    string cfgname;
    if( argc > 1){
        cfgname = argv[1];
    }else{
        cfgname = "Default";
    }

#ifdef DEBUG
    std::cout << "Reading config file: " << cfgname << std::endl;
#endif

    libconfig::Config cfg;
    try{
        cfg.readFile(cfgname.c_str());
    } catch ( const libconfig::ParseException& e){
        std::cerr << e.what() << std::endl;
        std::cerr << e.getError() << std::endl;
        std::cerr << "Line: " << e.getLine() << std::endl;
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG
    std::cout << "Config file read success." << std::endl;
#endif

    // TODO: Need a better way to build grid.
    // Intending to separate construction of grid from
    // the grid itself.
    // Set up grid
    std::cout << "Building grid.." << std::endl;
    Setting& gridCfg = cfg.lookup("Grid");
    unsigned int Ns[2];
    double Ls[2];
    Ns[0] = gridCfg.lookup("cells.x");
    Ns[1] = gridCfg.lookup("cells.y");
    Ls[0] = gridCfg.lookup("size.x");
    Ls[1] = gridCfg.lookup("size.y");
    int ghosts = 2;
    Grid grid(Ls,Ns,ghosts);

    Controller mySolver(grid, cfg);
    mySolver.exec();

    return EXIT_SUCCESS;
}
