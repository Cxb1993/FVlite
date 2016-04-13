// Timer.hpp
//
// Stores time, timestep, maximum time, and update conditions.

#ifndef TIMER_HPP
#define TIMER_HPP

#include <cmath>

#include "Vectors/Grid.hpp"

namespace FVTD{

class Timer{

private:

    double mT;
    double mDt;
    double mCFL;
    double mTmax;

public:

    Timer( double CFL, double tmax, const Grid* pGrid);

    // Access functions
    double t();
    double dt();
    double CFL();
    double tmax();

    // Setters (shouldn't be needed, but for some problems this allows greater flexibility)
    void setTmax( double tmax);
    void setCFL( double CFL);

    // Incrementers
    void advance();
    void advance(double ratio); // incremenets by ratio*dt

    // Test whether t>tmax
    bool complete();

};

Timer::Timer( double CFL, double tmax, const Grid* pGrid) : mT(0), mCFL(CFL), mTmax(tmax){
    double ds = (pGrid->dx() < pGrid->dy()) ? pGrid->dx() : pGrid->dy();
    mDt = (ds*CFL) / (c_c*sqrt(2)); // Courant condition. c_c is speed of light, root 2 accounts for 2 dimensions
}

double Timer::t(){
    return mT;
}

double Timer::dt(){
    return mDt;
}

double Timer::CFL(){
    return mCFL;
}

double Timer::tmax(){
    return mTmax;
}

void Timer::setTmax(double tmax){
    mTmax = tmax;
    return;
}

void Timer::setCFL( double CFL){
    mCFL=CFL;
    return;
}

void Timer::advance(){
    mT += mDt;
    return;
}

void Timer::advance(double ratio){
    mT += ratio*mDt;
    return;
}

bool Timer::complete(){
    return ( mT >= mTmax);
}

}// Namespace closure
#endif /* TIMER_HPP */
