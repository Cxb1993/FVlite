// FT_Controller.hpp
//
// Defines a class which acts on solvers to solve Fourier transform problems.
// Acts on two solvers, one with a cosine source and one with a sin source.
// Combines these to simulate complex fields.

#ifndef FT_CONTROLLER_HPP
#define FT_CONTROLLER_HPP

#include <cstdlib>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <libconfig.h++>

#include "FVlite.hpp"
#include "FT_Module.hpp"

using std::string;
using libconfig::Config;
using libconfig::Setting;

namespace FVlite{

class FT_Controller{

protected:

    Config& mCfg;
    Solver mSolver;

public:

    FT_Controller( Config& cfg) : mCfg(cfg), mSolver(cfg) {}
    void exec();
};

void FT_Controller::exec(){

    // Temporary function: should be replaced with something sturdier
    // Perhaps implement an 'overseer' class that controls the solver, which data should be printed, when to print, etc.

    // Get length of a single period
    Setting& BoundCfg = mCfg.lookup("Boundaries");
    int BoundSize = BoundCfg.getLength();
    double freq = 0.;
    double T;
    for( int ii=0; ii<BoundSize; ii++){
        Setting& thisCfg = BoundCfg[ii];
        try{
            Setting& params = thisCfg.lookup("params");
            freq = params.lookup("frequency");
        } catch(const std::exception&) {}
    }
    if( freq == 0.){
        std::cerr << "ERROR: zero frequency" << std::endl;
        exit(EXIT_FAILURE);
    } else {
        T = 2*M_PI/freq;
    }

    // Get number of time steps in single period
    // Assumes constant timestep. Valid for free-space Maxwell's equations, but not always true otherwise.
    int N = ceil(T/mSolver.pTimer->dt());
    
    std::cout << "Frequency: " << freq << std::endl;
    std::cout << "Period: " << T << std::endl;
    std::cout << "Timestep: " << mSolver.pTimer->dt() << std::endl;
    std::cout << "Number of timesteps per period: " << N << std::endl;

    // Determine number of cells in the near-field near PEC
    int n_cells = 0;
    int x_start = mSolver.pGrid->startX();
    int x_end   = mSolver.pGrid->endX();
    int y_start = mSolver.pGrid->startY();
    int y_end   = mSolver.pGrid->endY();
    BoundaryGeometry Boundary;
    for( int ii=x_start; ii<x_end; ii++){
        for( int jj=y_start; jj<y_end; jj++){
            Boundary = mSolver.pGrid->boundary(ii,jj);
            if( Boundary.isCut()) n_cells++;
        }
    }

    // Populate phi vector in the order that near-field cells are detected
    std::vector<double> Phi(n_cells);
    double phi;
    double x, y;
    int kk=0;
    for( int ii=x_start; ii<x_end; ii++){
        for( int jj=y_start; jj<y_end; jj++){
            Boundary = mSolver.pGrid->boundary(ii,jj);
            if( Boundary.isCut()){
                x = (ii-mSolver.pGrid->bound()+0.5)*mSolver.pGrid->dx()-mSolver.pGrid->Lx()/2.;
                y = (jj-mSolver.pGrid->bound()+0.5)*mSolver.pGrid->dy()-mSolver.pGrid->Ly()/2.;
                phi = atan2(y,x) * 180 / M_PI;
                if(phi>=0.){
                    Phi[kk] = phi;
                } else {
                    Phi[kk] = 360 + phi;
                }
                kk++;
            }  
        }
    }

    // Advance 10 full periods. This is enough time for the system to reach steady state
    while( mSolver.pTimer->t() < 10*T ) mSolver.advance();

    // Set N to power of 2
    int new_N = 1;
    while(new_N<N) new_N*=2;
    mSolver.pTimer->setDt(T/new_N);
    N = new_N;
    std::cout << "Calibrated number of timesteps per period: " << N << std::endl;

    // Set up storage
    std::vector<double> allTimeDomain( N*n_cells); // Stores time domain data at all cells
    std::vector<double> TimeDomainReal(N); // Stores real time domain data at a single grid location
    std::vector<double> TimeDomainImag(N); // Stores imaginary time domain data at a single grid location (all zeroes)
    std::vector<double> FreqDomainReal(N); // Stores real frequency domain data at a single grid location
    std::vector<double> FreqDomainImag(N); // Stores imaginary frequency domain data at a single grid location
    std::vector<double> Amplitude(n_cells); // Stores amplitude of frequency domain data


    // Iterate over another full period, storing data at each time step
    int cells = 0;
    StateVector State;
    for( int timestep = 0; timestep < N; timestep++){
        if(timestep>0) mSolver.advance();
        for( int ii=x_start; ii<x_end; ii++){
            for( int jj=y_start; jj<y_end; jj++){
                Boundary = mSolver.pGrid->boundary(ii,jj);
                if( Boundary.isCut()){
                    State = mSolver.pGrid->state(ii,jj);
                    allTimeDomain[ cells*N + timestep] = State.Hz();
                    cells++;
                }  
            }
        }
        cells = 0;
    }

    // Fourier transform one point at a time and print;
    FT_Module FT(N);
    double realpart;
    double imagpart;
    std::ofstream PEC_test_file("PEC_test_file.dat");
    for( cells=0; cells<n_cells; cells++){
        
        // copy allTimeDomain into TimeDomain
        for( int timestep=0; timestep<N; timestep++){
            TimeDomainReal[timestep] = allTimeDomain[ cells*N + timestep];
            TimeDomainImag[timestep] = 0.;
        }
        
        // Perform Fourier transform
        FT.exec( TimeDomainReal, TimeDomainImag, FreqDomainReal, FreqDomainImag);
        realpart = 2*FreqDomainReal[1]/N;
        imagpart = 2*FreqDomainImag[1]/N;

        // Calculate amplitude
        Amplitude[cells] = sqrt(realpart*realpart + imagpart*imagpart);
    }

    // Get normalising constant
    double max=0.;
    for( cells=0; cells<n_cells; cells++){
        max = (Amplitude[cells] > max) ? Amplitude[cells] : max;
    }

    // Print normalised data
    for( cells=0; cells<n_cells; cells++){
        PEC_test_file << Phi[cells]           << '\t'
                      << Amplitude[cells]/max << std::endl;   
    }

    return;

}


}// Namespace closure
#endif
