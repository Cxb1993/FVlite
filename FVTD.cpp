// FVTD.cpp
//
// Code to run FVTD.hpp.
// Used to solve scattering problems
// Edit: Generalised to solve Euler equations too. Could do with a new name.

#include <cstdlib>
#include <string>

#include <libconfig.h++>

#include "FVTD.hpp"

#ifdef MAXWELL
#include "FT_Controller.hpp"
#endif

using namespace FVTD;
using std::string;

int main( void){

    libconfig::Config cfg;
    cfg.readFile("Default.cfg");

    Solver mySolver(cfg);
    mySolver.printData();
    mySolver.printGeometry();
//    mySolver.printLevelSetVertices();
//    mySolver.exec();

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
