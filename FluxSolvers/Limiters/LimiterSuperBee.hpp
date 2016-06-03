// LimiterSuperBee.hpp
//
// Finite volume grid updater limiter.
// SuperBee

#ifndef LIMITERSUPERBEE_HPP
#define LIMITERSUPERBEE_HPP

#include"LimiterAbstract.hpp"

namespace FVlite{

class LimiterSuperBee : public Limiter{
public:
    LimiterSuperBee(){}
    virtual StateVector exec(StateVector& r, double omega);
};

REGISTER(Limiter,SuperBee)

StateVector LimiterSuperBee::exec(StateVector& r, double omega){
    StateVector Xi;
    StateVector result;
    double tmp;
    Xi = Xi_R(r,omega);
    for( unsigned int ii=0; ii<r.size(); ii++){
        if(r[ii]<0){
            tmp = 0;
        } else if(r[ii]<=0.5){
            tmp = 2*r[ii];
        } else if(r[ii]<=1){
            tmp = 1.0;
        } else {
            tmp = (r[ii]<Xi[ii]) ? r[ii] : Xi[ii];
            tmp = (tmp<2.) ? tmp : 2.;
        }
        result[ii] = tmp;
    }
    return result;
}

}// Namespace closure
#endif /* LIMITERSUPERBEE_HPP */
