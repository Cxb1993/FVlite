// Timer.hpp
//
// Stores time, timestep, maximum time, and update conditions.

#ifndef TIMER_HPP
#define TIMER_HPP

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <libconfig.h++>

using libconfig::Setting;

namespace FVlite{

class Timer{

private:

    double mT;
    double mDt;
    double mCFL;
    double mTmax;

public:

    Timer(){}

    void init( Setting& cfg); 

    // Access functions
    inline double t(){ return mT;}
    inline double dt(){ return mDt;}
    inline double cfl(){ return mCFL;}
    inline double tmax(){ return mTmax;}

    // Setters (shouldn't be needed, but for some problems this allows greater flexibility)
    inline void set_dt( double dt){ mDt = dt; return;}
    inline void set_tmax( double tmax){ mTmax = tmax; return;}
    inline void set_cfl( double CFL){ mCFL = CFL; return;}

    // Incrementers
    inline void advance(double ratio); // incremenets by ratio*dt
    inline void advance(){ advance(1.0); return;}

    // Test whether t>tmax
    inline bool is_complete(){ return (mT>=mTmax);}
};

void Timer::init( Grid* pGrid, Setting& cfg){
    mT = 0;
    mDt = 0;
    mCFL  = cfg.lookup("CFL");
    mTmax = cfg.lookup("tmax");
}

void Timer::advance(double ratio){ 
    if( mDt == 0.0 ){
        std::cerr << "ERROR: TIMESTEP SET TO ZERO" << std::endl;
        exit(EXIT_FAILURE);
    }
    mT+=ratio*mDt;
}

}// Namespace closure
#endif /* TIMER_HPP */
