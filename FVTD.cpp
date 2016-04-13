// FVTD.cpp
//
// Code to run FVTD.hpp.
// Used to solve scattering problems

#include<string>

#include"FVTD.hpp"

using namespace FVTD;

int main( void){

    std::string runName("data/results");
    int           Nx = 100;
    int           Ny = 100;
    double        Lx = 1.0;
    double        Ly = 1.0;
    double        CFL = 0.01;
    double        tmax = 1e-9;
    INIT_TYPE     ITYPE = ZERO;
    PDE_TYPE      PTYPE = FORCE;
    B_UPDATE_TYPE BTYPE = TRANSMISSIVE;
    LIMIT_TYPE    LTYPE = MINBEE;

    Solver mySolver( runName, Nx, Ny, Lx, Ly, CFL, tmax, ITYPE, PTYPE, BTYPE, LTYPE);

    mySolver.exec();
/*
    std::string prefix("data/results");
    std::string suffix(".dat");
    std::string number;
    std::string filename;
    std::ofstream file;
    int iter = 0;
    while(!mySolver.complete()){
        iter++;
        mySolver.advance();
        if(iter%1==0){
            number = std::to_string(iter/1);
            filename = prefix+number+suffix;
            file.open(filename.c_str());
            mySolver.printData(file);
            file.close();
        }
    }
*/
    return 0;
}
