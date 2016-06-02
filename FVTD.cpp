// FVTD.cpp
//
// Code to run FVTD.hpp.
// Used to solve scattering problems
// Edit: Generalised to solve Euler equations too. Could do with a new name.

#include <cstdlib>
#include <string>

#include "FVTD.hpp"

#ifdef MAXWELL
#include "FT_Controller.hpp"
#endif

using namespace FVTD;
using std::string;

int main( void){

    string  runName("data/results");
    int     Nx = 200;
    int     Ny = Nx;
    double  Lx = 2.0;
    double  Ly = Lx;
    double  CFL = 0.9;
#ifdef MAXWELL
    double  tmax = 5e-9;
#endif
#ifdef EULER
    double  tmax = 1.0;
#endif
    string InitString = "ExplosiveTest";
    string FluxString = "HLLC";
    string FvmString = "Std";
    string buString = "Transmissive";
    string LimitString = "VanLeer";
    SOURCE_TYPE   STYPE = SINE;

    Solver mySolver( runName, Nx, Ny, Lx, Ly, CFL, tmax, InitString, FvmString, FluxString, buString, LimitString, STYPE);
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
