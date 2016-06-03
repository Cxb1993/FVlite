// FT_Controller.hpp
//
// Defines a class which acts on solvers to solve Fourier transform problems.
// Acts on two solvers, one with a cosine source and one with a sin source.
// Combines these to simulate complex fields.

#ifndef FT_CONTROLLER_HPP
#define FT_CONTROLLER_HPP

#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

#include "FVlite.hpp"
#include "FT_Module.hpp"

namespace FVlite{

class FT_Controller{

protected:

    Solver RealSolver;
    Solver ImagSolver;

    Solver& R;
    Solver& I;

public:

    FT_Controller( int Nx, int Ny, double Lx, double Ly, double CFL, double tmax,
                   INIT_TYPE ITYPE, FVM_TYPE FVMTYPE, FLUX_TYPE FTYPE, B_UPDATE_TYPE BTYPE, LIMIT_TYPE LTYPE=NONE)
        : RealSolver("data/results",Nx,Ny,Lx,Ly,CFL,tmax,ITYPE,FVMTYPE,FTYPE,BTYPE,LTYPE,SINE),
          ImagSolver("data/results",Nx,Ny,Lx,Ly,CFL,tmax,ITYPE,FVMTYPE,FTYPE,BTYPE,LTYPE,COSINE),
          R(RealSolver), I(ImagSolver)
    {}

    void exec();
};

void FT_Controller::exec(){

    // Temporary function: should be replaced with something sturdier
    // Perhaps implement an 'overseer' class that controls the solver, which data should be printed, when to print, etc.
    

    double T = 2*M_PI/omega;       // single period
    int N = ceil(T/R.pTimer->dt()); // Number of time steps in single period
    
    std::cout << "Number of timesteps per period: " << N << std::endl;

    // Determine number of cells in the near-field near PEC
    int n_cells = 0;
    int x_start = R.pGrid->startX();
    int x_end   = R.pGrid->endX();
    int y_start = R.pGrid->startY();
    int y_end   = R.pGrid->endY();
    for( int ii=x_start; ii<x_end; ii++){
        for( int jj=y_start; jj<y_end; jj++){
            // if grid cell not PEC, but at least one neighbour is...
            /*if( !R.pGrid->material(ii,jj).PEC() && (  R.pGrid->material(ii+1,jj).PEC() 
                                                   || R.pGrid->material(ii-1,jj).PEC()
                                                   || R.pGrid->material(ii,jj+1).PEC()
                                                   || R.pGrid->material(ii,jj-1).PEC() )){*/
            if( R.pGrid->levelset(ii,jj) < 0 && (  R.pGrid->levelset(ii+1,jj) >= 0 
                                                || R.pGrid->levelset(ii-1,jj) >= 0
                                                || R.pGrid->levelset(ii,jj+1) >= 0
                                                || R.pGrid->levelset(ii,jj-1) >= 0 )){
                n_cells++;
            }  
        }
    }

    // Populate phi vector in the order that near-field cells are detected
    std::vector<double> Phi(n_cells);
    double phi;
    double x, y;
    int kk=0;
    for( int ii=x_start; ii<x_end; ii++){
        for( int jj=y_start; jj<y_end; jj++){
            // if grid cell not PEC, but at least one neighbour is...
            /*if( !R.pGrid->material(ii,jj).PEC() && (  R.pGrid->material(ii+1,jj).PEC() 
                                                   || R.pGrid->material(ii-1,jj).PEC()
                                                   || R.pGrid->material(ii,jj+1).PEC()
                                                   || R.pGrid->material(ii,jj-1).PEC() )){*/
            if( R.pGrid->levelset(ii,jj) < 0 && (  R.pGrid->levelset(ii+1,jj) >= 0 
                                                || R.pGrid->levelset(ii-1,jj) >= 0
                                                || R.pGrid->levelset(ii,jj+1) >= 0
                                                || R.pGrid->levelset(ii,jj-1) >= 0 )){
                x = (ii-R.pGrid->bound()+0.5)*R.pGrid->dx()-R.pGrid->Lx()/2.;
                y = (jj-R.pGrid->bound()+0.5)*R.pGrid->dy()-R.pGrid->Ly()/2.;
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
    while( R.pTimer->t() < 10*T ) R.advance();
    //while( I.pTimer->t() < 10*T ) I.advance();

    // Set N to power of 2
    int new_N = 1;
    while(new_N<N) new_N*=2;
    R.pTimer->setDt(T/new_N);
    N = new_N;
    std::cout << "Calibrated number of timesteps per period: " << N << std::endl;

    // Set up storage
    std::vector<double> allTimeDomainReal( N*n_cells); // Stores real time domain data at all cells
    std::vector<double> allTimeDomainImag( N*n_cells); // Stores imaginary time domain data at all cells
    std::vector<double> TimeDomainReal(N); // Stores real part of time domain data at a single grid location
    std::vector<double> TimeDomainImag(N); // Stores imaginary part of time domain data at a single grid location
    std::vector<double> FreqDomainReal(N); // Stores real part of frequency domain data
    std::vector<double> FreqDomainImag(N); // Stores imaginary part of frequency domain data


    // Iterate over another full period, storing data at each time step
    
    int cells = 0;
    StateVector StateReal;
    StateVector StateImag;
    for( int timestep = 0; timestep < N; timestep++){
        if(timestep>0){
            R.advance();
        //    I.advance();
        }
        for( int ii=x_start; ii<x_end; ii++){
            for( int jj=y_start; jj<y_end; jj++){
                // if grid cell not PEC, but at least one neighbour is...
                /*if( !R.pGrid->material(ii,jj).PEC() && ( R.pGrid->material(ii+1,jj).PEC() 
                                                      || R.pGrid->material(ii-1,jj).PEC()
                                                      || R.pGrid->material(ii,jj+1).PEC()
                                                      || R.pGrid->material(ii,jj-1).PEC() )){*/
                if( R.pGrid->levelset(ii,jj) < 0 && (  R.pGrid->levelset(ii+1,jj) >= 0 
                                                    || R.pGrid->levelset(ii-1,jj) >= 0
                                                    || R.pGrid->levelset(ii,jj+1) >= 0
                                                    || R.pGrid->levelset(ii,jj-1) >= 0 )){
                    StateReal = R.pGrid->state(ii,jj);
                    StateImag = I.pGrid->state(ii,jj);
                    allTimeDomainReal[ cells*N + timestep] = StateReal.Hz();
                    allTimeDomainImag[ cells*N + timestep] = 0;//StateImag.Hz();
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
            TimeDomainReal[timestep] = allTimeDomainReal[ cells*N + timestep];
            TimeDomainImag[timestep] = allTimeDomainImag[ cells*N + timestep];
        }
        
        // Perform Fourier transform
        FT.exec( TimeDomainReal, TimeDomainImag, FreqDomainReal, FreqDomainImag);
        realpart = 2*FreqDomainReal[1]/N;
        imagpart = 2*FreqDomainImag[1]/N;

        // print
        PEC_test_file << Phi[cells] << '\t'
                      << realpart   << '\t'
                      << imagpart   << '\t'
                      << sqrt(realpart*realpart + imagpart*imagpart) << std::endl;   
    }

    // Also print time domain response at end of run

    R.printData();

    return;

}


}// Namespace closure
#endif
