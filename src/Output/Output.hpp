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
#include "Timer/Timer.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class Output{

private:

    int mCounter;        // Counts number of timesteps.
    int mPrintEvery;     // Print on every mPrintEvery'th timestep. Set to zero to print only final results.
    double mPrintDelay;  // Time must be greater than print delay to start printing
    bool mPrintInitial;  // Print initial conditions?
    bool mPrintCutCells; // Print only cut cells
    string mRunName;     // Name of current run.

    Grid* mpGrid;        // Pointer to grid.
    Timer* mpTimer;      // Pointer to timer

public:

    Output(){}
    ~Output(){}

    void init( Grid* pGrid, Timer* pTimer, const Setting& cfg);
    void prod();
    void print();
    void print_geometry();

};

// Function definitions

void Output::init( Grid* pGrid, Timer* pTimer, const Setting& cfg){
    mCounter = 0;
    mPrintEvery = cfg.lookup("PrintEvery");
    mPrintDelay = cfg.lookup("PrintDelay");
    mPrintInitial = cfg.lookup("PrintInitial");
    mPrintCutCells = cfg.lookup("PrintCutCells");
    mRunName = cfg.lookup("RunName").c_str();
    mpGrid = pGrid;
    mpTimer = pTimer;
    return;
}

void Output::prod(){
    double t = mpTimer->t();
    if(  (mCounter==0 && mPrintInitial) 
      || (mPrintEvery!=0 && mCounter>0 && 
             ( mCounter % mPrintEvery == 0 ) &&
             ( t >= mPrintDelay )
         ) 
      )
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
    BoundaryGeometry Boundary;
    double alpha;
//    for( int ii=mpGrid->startX(); ii<mpGrid->endX(); ii++){
//        for( int jj=mpGrid->startY(); jj<mpGrid->endY(); jj++){
    for( unsigned int ii=0; ii<mpGrid->full_width(DIM_X); ii++){
        for( unsigned int jj=0; jj<mpGrid->full_width(DIM_Y); jj++){

            Boundary = mpGrid->boundary(ii,jj);
            alpha = Boundary.alpha();
            if( !mPrintCutCells || (alpha!=0. && alpha!=1.)){

                // Print position
                File << mpGrid->position(DIM_X,ii)  << '\t' << mpGrid->position(DIM_Y,jj)  << '\t';

                // Print conserved data
                State = mpGrid->state(ii,jj);
                for( unsigned int kk=0; kk<State.size(); kk++){
                    File << State[kk] << '\t';
                }
            
#ifdef DEBUG
                // Print auxiliary data
                State = mpGrid->boundary_state(ii,jj);
                for( unsigned int kk=0; kk<State.size(); kk++){
                    File << State[kk] << '\t';
                }
#endif

                // Print level set function
                File << mpGrid->levelset(ii,jj) << std::endl;
            }

        }
        // Blank line, for gnuplot
        File << std::endl;
    }

    // Close file

    File.close();

    return;
}

void Output::print_geometry(){

    // Get filename

    string Prefix = "data/";
    string Suffix = "Geometry.dat";
    string Filename = Prefix+mRunName+Suffix;

    // Open file
    
    std::ofstream File( Filename);

    // Print metadata

    File << "# ii" << std::endl;
    File << "# jj" << std::endl;
    File << "# x" << std::endl;
    File << "# y" << std::endl;

    File << "# betaL" << std::endl;
    File << "# betaR" << std::endl;
    File << "# betaT" << std::endl;
    File << "# betaB" << std::endl;
    File << "# alpha" << std::endl;
    File << "# Nb[0]" << std::endl;
    File << "# Nb[1]" << std::endl;
    File << "# Nb[2]" << std::endl;

    // Print data

    BoundaryGeometry Boundary;
    for( unsigned int ii=0; ii<mpGrid->full_width(DIM_X); ii++){
        for( unsigned int jj=0; jj<mpGrid->full_width(DIM_Y); jj++){

            // Print grid location
            File << ii  << '\t' << jj << '\t';

            // Print position
            File << mpGrid->position(DIM_X,ii)  << '\t' << mpGrid->position(DIM_Y,jj)  << '\t';

            // Print boundary data
            Boundary = mpGrid->boundary(ii,jj); 
            File << Boundary.betaL() << '\t';
            File << Boundary.betaR() << '\t';
            File << Boundary.betaT() << '\t';
            File << Boundary.betaB() << '\t';
            File << Boundary.alpha() << '\t';
            File << (Boundary.Nb())[0] << '\t';
            File << (Boundary.Nb())[1] << '\t';
            File << (Boundary.Nb())[2] << '\t';
            File << std::endl;

        }
        File << std::endl;
    }

    // Close file

    File.close();

    return;

}

}// Namespace closure
#endif /* OUTPUT_HPP */
