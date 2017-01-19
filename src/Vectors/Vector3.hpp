// Vector3.hpp
//
// Uses MathVector to define a 3D vector 

#ifndef VECTORTHREE_HPP
#define VECTORTHREE_HPP

#include <cmath>
#include "MathVector.hpp"

namespace FVlite{

class Vector3 : public MathVector<3>{

public:
    using MathVector<3>::operator=;

    Vector3() : MathVector() {}
    Vector3( double x, double y, double z);

    inline void set( double x, double y, double z);

    inline double x() const{ return mX[0];}
    inline double y() const{ return mX[1];}
    inline double z() const{ return mX[2];}
    inline double& x(){ return mX[0];}
    inline double& y(){ return mX[1];}
    inline double& z(){ return mX[2];}
};

Vector3::Vector3( double x, double y, double z){
    set(x,y,z);
}

void Vector3::set( double x, double y, double z){
    mX[0] = x;
    mX[1] = y;
    mX[2] = z;
    return;
}

}// Namespace closure
#endif /* VECTORTHREE_HPP */
