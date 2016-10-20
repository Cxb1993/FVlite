// FVlite.cpp
//
// Code to run FVlite.hpp.
// Used to solve finite volume problems.
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
    } catch ( const std::exception& e){
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG
    std::cout << "Config file read success." << std::endl;
#endif

    Solver mySolver(cfg);
    mySolver.exec();

    return EXIT_SUCCESS;
}
