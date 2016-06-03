// Timer.hpp
//
// Stores time, timestep, maximum time, and update conditions.

#ifndef TIMER_HPP
#define TIMER_HPP

#include <cstdlib>
#include <cmath>
#include <iostream>

#include "Grid/Grid.hpp"

namespace FVlite{

class Timer{

private:

    double mT;
    double mDt;
    double mCFL;
    double mTmax;

    Grid* pGrid;

public:

    Timer( double CFL, double tmax, Grid* pGrid);

    // Access functions
    inline double t(){ return mT;}
    inline double dt(){ return mDt;}
    inline double CFL(){ return mCFL;}
    inline double tmax(){ return mTmax;}

    // Setters (shouldn't be needed, but for some problems this allows greater flexibility)
    inline void setDt( double dt){ mDt = dt; return;}
    inline void setTmax( double tmax){ mTmax = tmax; return;}
    inline void setCFL( double CFL){ mCFL = CFL; return;}

    // Incrementers
    inline void advance(double ratio); // incremenets by ratio*dt
    inline void advance(){ advance(1.0); return;}

    // Test whether t>tmax
    inline bool complete(){ return (mT>=mTmax);}

    // Set timestep according to maximum grid speed
    inline void calibrate_timestep();
};

Timer::Timer( double CFL, double tmax, Grid* pGrid) : mT(0), mDt(0), mCFL(CFL), mTmax(tmax), pGrid(pGrid){}

void Timer::advance(double ratio){ 
    if( mDt == 0.0 ){
        std::cerr << "ERROR: TIMESTEP SET TO ZERO" << std::endl;
        exit(EXIT_FAILURE);
    }
    mT+=ratio*mDt;
    return;
}

void Timer::calibrate_timestep(){
    double maxSpeed = pGrid->maxSpeed();
    double ds = (pGrid->dx() < pGrid->dy()) ? pGrid->dx() : pGrid->dy();
    mDt = (ds*mCFL) / (maxSpeed*sqrt(2)); // Courant condition, root 2 accounts for 2 dimensions
    return;
}

}// Namespace closure
#endif /* TIMER_HPP */
