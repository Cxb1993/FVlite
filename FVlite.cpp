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

//#ifdef MAXWELL
//#include "FT_Controller.hpp"
//#endif

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
//    mySolver.printData();
//    mySolver.printGeometry();
//    mySolver.printLevelSetVertices();
    mySolver.exec();

//    FT_Controller myFT( Nx, Ny, Lx, Ly, CFL, tmax, ITYPE, FVMTYPE, FTYPE, BTYPE, LTYPE);
//    myFT.exec();
/*
    std::string prefix("data/results");
    std::string suffix(".dat");
    std::string number;
    std::string filename;
    std::ofstream file;
    int iter = 0;
    int print_every = 10;
    while(!mySolver.complete()){
        iter++;
        mySolver.advance();
        if(iter%print_every==0){
            number = std::to_string(iter/print_every);
            filename = prefix+number+suffix;
            file.open(filename.c_str());
            mySolver.printData(file);
            file.close();
        }
    }
*/
    return EXIT_SUCCESS;
}
