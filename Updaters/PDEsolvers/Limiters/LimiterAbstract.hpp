// LimiterAbstract.hpp
//
// Abstract finite volume grid boundary updater limiter.

#ifndef LIMITERABSTRACT_HPP
#define LIMITERABSTRACT_HPP

#include"Vectors/Grid.hpp"

namespace FVTD{

class Limiter{
public:

    Limiter();
    virtual ~Limiter();

    virtual StateVector exec( StateVector& r, double omega)=0;

    StateVector Xi_R( StateVector & r, double omega);
};

Limiter::Limiter(){
}

Limiter::~Limiter(){
}

StateVector Limiter::Xi_R( StateVector& r, double omega){
    StateVector denom;
    StateVector result;
    denom = 1-omega + (1+omega)*r;
    result = 2.0/denom;
    return result;
}

}// Namespace closure
#endif /* LIMITERABSTRACT_HPP */