// LimiterNone.hpp
//
// Finite volume grid updater limiter.
// No limiter applied

#ifndef LIMITERNONE_HPP
#define LIMITERNONE_HPP

#include"LimiterAbstract.hpp"

namespace FVlite{

class LimiterNone : public Limiter{
public:
    LimiterNone(){}
    virtual StateVector exec(StateVector& r, double omega);
};

REGISTER(Limiter,None)

StateVector LimiterNone::exec(StateVector& r, double omega){
    (void)r; // Prevents unused parameter warnings
    (void)omega;
    StateVector result;
    for( unsigned int ii=0; ii<r.size(); ii++){
        result[ii] = 1.0;
    }
    return result;
}

}// Namespace closure
#endif /* LIMITERNONE_HPP */
