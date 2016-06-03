// Output.hpp
//
// Controls printing to file and similar functions.
// Should be prodded every time step.

#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <libconfig.h++>

#include "Grid/Grid.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class Output{

private:

    int mCounter;        // Counts number of timesteps.
    int mPrintEvery;     // Print on every mPrintEvery'th timestep. Set to zero to print only final results.
    bool mPrintInitial;  // Print initial conditions?
    string mRunName;     // Name of current run.
    Grid* mpGrid;        // Pointer to grid.

public:

    Output(){}
    ~Output(){}

    void init( Grid* pGrid, const Setting& cfg);
    void prod();
    void print();

};

// Function definitions

void Output::init( Grid* pGrid, const Setting& cfg){
    mCounter = 0;
    mPrintEvery = cfg.lookup("PrintEvery");
    mPrintInitial = cfg.lookup("PrintInitial");
    mRunName = cfg.lookup("RunName").c_str();
    mpGrid = pGrid;
    std::cout << "Print every: " << mPrintEvery << std::endl;
    std::cout << "Print initial: " << mPrintInitial << std::endl;
    return;
}

void Output::prod(){
    if(  (mCounter==0 && mPrintInitial) 
      || (mPrintEvery!=0 && mCounter>0 && ( mCounter % mPrintEvery == 0  )) )
    {
        print();
    }
    mCounter++;
    return;
}

void Output::print(){

    // Get filename

    string Prefix = "data/";
    string Suffix = ".dat";
    string Filename;
    if( mCounter==0 && mPrintInitial){
        string InitString = "Init";
        Filename = Prefix+mRunName+InitString+Suffix;
    }
    if( mPrintEvery!=0 && mCounter>0){
        string CountString = std::to_string(mCounter/mPrintEvery);
        Filename = Prefix+mRunName+CountString+Suffix;
    }
    if( mPrintEvery==0 && mCounter>0){
        Filename = Prefix+mRunName+Suffix;
    }

    // Open file
    
    std::ofstream File( Filename);

    // Print metadata

    File << "# x" << std::endl;
    File << "# y" << std::endl;

#ifdef MAXWELL
    File << "# Ex" << std::endl;
    File << "# Ey" << std::endl;
    File << "# Ez" << std::endl;
    File << "# Hx" << std::endl;
    File << "# Hy" << std::endl;
    File << "# Hz" << std::endl;
#endif

#ifdef EULER
    File << "# rho" << std::endl;
    File << "# rho * ux" << std::endl;
    File << "# rho * uy" << std::endl;
    File << "# E" << std::endl;
#endif

    File << "# Level set" << std::endl;
    
    // Print data

    StateVector State;
    for( int ii=mpGrid->startX(); ii<mpGrid->endX(); ii++){
        for( int jj=mpGrid->startY(); jj<mpGrid->endY(); jj++){

            // Print position
            File << mpGrid->x(ii)  << '\t' << mpGrid->y(jj)  << '\t';

            // Print conserved data
            State = mpGrid->state(ii,jj);
            for( unsigned int kk=0; kk<State.size(); kk++){
                File << State[kk] << '\t';
            }

            // Print level set function
            File << (*mpGrid->levelset())(ii,jj) << std::endl;

        }
        File << std::endl;
    }

    // Close file

    File.close();

    return;
}

}// Namespace closure
#endif /* OUTPUT_HPP */
