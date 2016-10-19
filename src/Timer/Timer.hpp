// Timer.hpp
//
// Stores time, timestep, maximum time, and update conditions.

#ifndef TIMER_HPP
#define TIMER_HPP

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <libconfig.h++>

#include "Grid/Grid.hpp"

using libconfig::Setting;

namespace FVlite{

class Timer{

private:

    double mT;
    double mDt;
    double mCFL;
    double mTmax;

    Grid* mpGrid;

public:

    Timer(){}

    void init( Grid* pGrid, Setting& cfg); 

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
    inline bool is_complete(){ return (mT>=mTmax);}

    // Set timestep according to maximum grid speed
    inline void calibrate_timestep();
};

void Timer::init( Grid* pGrid, Setting& cfg){
    mT = 0;
    mDt = 0;
    mCFL  = cfg.lookup("CFL");
    mTmax = cfg.lookup("tmax");
    mpGrid = pGrid;
    return;
}

void Timer::advance(double ratio){ 
    if( mDt == 0.0 ){
        std::cerr << "ERROR: TIMESTEP SET TO ZERO" << std::endl;
        exit(EXIT_FAILURE);
    }
    mT+=ratio*mDt;
    return;
}

void Timer::calibrate_timestep(){
    double maxSpeed = mpGrid->maxSpeed();
    double ds = (mpGrid->dx() < mpGrid->dy()) ? mpGrid->dx() : mpGrid->dy();
    mDt = (ds*mCFL)/maxSpeed; // Courant condition
    return;
}

}// Namespace closure
#endif /* TIMER_HPP */
