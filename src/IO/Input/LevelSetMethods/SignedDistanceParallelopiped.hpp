// SignedDistanceParallelepiped.hpp
//
// Returns signed distance from a parallelepiped.
// Though a bit messy, it may be used to determine the distance
// from simpler shapes like rectanlges and cubes.
//
// Vector r refers to a corner.
// Vectors b and c define a parallelogram from that corner
// Vector a makes a parallelepiped.
// Area given by a.(bxc)

#ifndef SIGNEDDISTANCEPARALLELEPIPED_HPP
#define SIGNEDDISTANCEPARALLELEPIPED_HPP

#include "SignedDistanceAbstract.hpp"

using libconfig::Setting;

namespace FVlite{

class SignedDistanceParallelepiped : public SignedDistance {

protected:

    Vector3 m_r;
    Vector3 m_a;
    Vector3 m_b;
    Vector3 m_c;

public:

    SignedDistanceParallelepiped(){}
    virtual void init( Setting& cfg);
    virtual double get_distance( const Vector3& pos) const;

};

REGISTER(SignedDistance,Parallelepiped)


void SignedDistanceParallelepiped::init( Setting& cfg){
    Vector3 r( 0., 0., 0.,), a( 0., 0., 0.), b(0., 0., 0.), c( 0., 0., 0.);
    mLowerLeftX = cfg.lookup("lowerleft.x");
    mLowerLeftY = cfg.lookup("lowerleft.y");
    mUpperRightX = cfg.lookup("upperright.x");
    mUpperRightY = cfg.lookup("upperright.y");
    return;
}


double InitialisationModuleRectangle::exec( double x, double y){
    double levelset=0;
    double LLx = mLowerLeftX;
    double LLy = mLowerLeftY;
    double URx = mUpperRightX;
    double URy = mUpperRightY;
    // Check each possible area of grid
    // Left side
    if( x < LLx){
        // Lower
        if( y < LLy){
            levelset = -sqrt( (x-LLx)*(x-LLx) + (y-LLy)*(y-LLy));
        }
        // Central
        if( y >= LLy && y <= URy){
            levelset = x - LLx;
        }
        // Upper
        if( y > URy){
            levelset = -sqrt( (x-LLx)*(x-LLx) + (y-URy)*(y-URy));
        }
    }
    // Central region
    if( x >= LLx && x <= URx){
        // Lower
        if( y < LLy){
            levelset = y - LLy;
        }
        // Central
        if( y >= LLy && y <= URy){
            levelset = fmin( x-LLx, URx-x);
            levelset = ( fmin( y-LLy, URy-y) < levelset) ? fmin(y-LLy,URy-y) : levelset;
        }
        // Upper
        if( y > URy){
            levelset = URy - y;
        }
    }
    // Right side
    if( x > URx){
        // Lower
        if( y < LLy){
            levelset = -sqrt( (x-URx)*(x-URx) + (y-LLy)*(y-LLy));
        }
        // Central
        if( y >= LLy && y <= URy){
            levelset = URx - x;
        }
        // Upper
        if( y > URy){
            levelset = -sqrt( (x-URx)*(x-URx) + (y-URy)*(y-URy));
        }
    }
    return levelset;
}

}// Namespace closure
#endif /* INITIALISATIONMODULERECTANGLE_HPP */
