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

#ifdef MAXWELL
#include "FT_Controller.hpp"
#endif

using namespace FVlite;
using std::string;

int main( int argc, char* argv[]){

    string cfgname;
    if( argc > 1){
        cfgname = argv[1];
    }else{
        cfgname = "Default";
    }

    cfgname = cfgname + ".cfg";

#ifdef DEBUG
    std::cout << "Reading config file: " << cfgname << std::endl;
#endif

    libconfig::Config cfg;
    cfg.readFile(cfgname.c_str());

#ifdef DEBUG
    std::cout << "Config file read success." << std::endl;
#endif

    Solver mySolver(cfg);
    mySolver.exec();

    // TODO
    // Update Output such that Fourier transforms can be selected at run time.
    // Perhaps get cut cell output to work, and write external scripts that process data
    // an appropriate form. Then pass that to a FFT library (there must be one in SciPy),
    // and plot from there.
//    FT_Controller myFT( cfg);
//    myFT.exec();

    return EXIT_SUCCESS;
}
