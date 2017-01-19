// SignedDistanceSphere.hpp
//
// Get signed distance from sphere-like object.
// Should be used for circles and whatever the equivalent shape is in 1D.

#ifndef SIGNEDDISTANCESPHERE_HPP
#define SIGNEDDISTANCESPHERE_HPP

#include <cmath>

#include "SignedDistanceAbstract.hpp"
#include "Vectors/Vector3.hpp"

using libconfig::Setting;

namespace FVlite{

class SignedDistanceSphere : public SignedDistance {

protected:

    Vector3 m_center;
    double m_radius;

public:

    SignedDistanceSphere(){}
    virtual void init( Setting& cfg);
    virtual double get_distance( const Vector3& pos) const;

};

REGISTER(SignedDistance,Sphere)

void SignedDistanceSphere::init( Setting& cfg){
    Vector3 center( 0.0, 0.0, 0.0);
    try{ center[0] = cfg.lookup("center.x");}
    catch( const std::exception& e){}
    try{ center[1] = cfg.lookup("center.y");}
    catch( const std::exception& e){}
    try{ center[2] = cfg.lookup("center.z");}
    catch( const std::exception& e){}
    m_center = center;
    m_radius = cfg.lookup("radius");
}


double SignedDistanceSphere::get_distance( const Vector3& pos) const{
    return m_radius - Geometry::distance_point_point(m_center,pos);
}

}
#endif
