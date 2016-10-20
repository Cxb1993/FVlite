// LimiterDefault.hpp
//
// Finite volume slope limiter.
// trivial case - no limiter applied

#ifndef LIMITERDEFAULT_HPP
#define LIMITERDEFAULT_HPP

#include"LimiterAbstract.hpp"

namespace FVlite{

class LimiterDefault : public Limiter{
public:
    LimiterDefault(){}
    virtual StateVector exec(StateVector& r, double omega);
};

REGISTER(Limiter,Default)

StateVector LimiterDefault::exec(StateVector& r, double omega){
    (void)r; // Prevents unused parameter warnings
    (void)omega;
    StateVector result;
    for( unsigned int ii=0; ii<r.size(); ii++){
        result[ii] = 1.0;
    }
    return result;
}

}// Namespace closure
#endif /* LIMITERDEFAULT_HPP */
