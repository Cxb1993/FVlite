// LimiterMinBee.hpp
//
// Finite volume grid updater limiter.
// MinBee

#ifndef LIMITERMINBEE_HPP
#define LIMITERMINBEE_HPP

#include"LimiterAbstract.hpp"

namespace FVTD{

class LimiterMinBee : public Limiter{
public:
    LimiterMinBee(){}
    virtual StateVector exec( StateVector& r, double omega);
};

REGISTER(Limiter,MinBee)

StateVector LimiterMinBee::exec(StateVector& r, double omega){
    StateVector Xi;
    StateVector result;
    double tmp;
    Xi = Xi_R(r,omega);
    for( unsigned int ii=0; ii<r.size(); ii++){
        if(r[ii]<0){
            tmp = 0;
        } else if(r[ii]<=1.0){
            tmp = r[ii];
        } else {
            tmp = (1<Xi[ii]) ? 1. : Xi[ii];
        }
        result[ii] = tmp;
    }
    return result;
}

}// Namespace closure
#endif /* LIMITERMINBEE_HPP */
