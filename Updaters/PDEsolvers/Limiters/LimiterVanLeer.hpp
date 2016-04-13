// LimiterVanLeer.hpp
//
// Finite volume grid updater limiter.
// VanLeer

#ifndef LIMITERVANLEER_HPP
#define LIMITERVANLEER_HPP

#include"LimiterAbstract.hpp"

namespace FVTD{

class LimiterVanLeer : public Limiter{
public:
    LimiterVanLeer() : Limiter() {}
    virtual StateVector exec(StateVector& r, double omega);
};

StateVector LimiterVanLeer::exec(StateVector& r, double omega){
    StateVector Xi;
    StateVector result;
    double tmp;
    double tmp2;
    Xi = Xi_R(r,omega);
    for( unsigned int ii=0; ii<r.size(); ii++){
        if(r[ii]<0){
            tmp = 0;
        } else {
            tmp2 = 2*r[ii]/(1.+r[ii]);
            tmp = (tmp2<Xi[ii]) ? tmp2 : Xi[ii];
        }
        result[ii] = tmp;
    }
    return result;
}

}// Namespace closure
#endif /* LIMITERVANLEER_HPP */
