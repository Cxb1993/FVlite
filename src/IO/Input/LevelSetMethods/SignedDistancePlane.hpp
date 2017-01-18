// SignedDistancePlane.hpp
//
// Provides signed distance from a plane.
// Normal points towards negative signed distance.

#ifndef SIGNEDDISTANCEPLANE_HPP
#define SIGNEDDISTANCEPLANE_HPP

#include "SignedDistanceAbstract.hpp"
#include "Vectors/Vector3.hpp"

using libconfig::Setting;

namespace FVlite{

class SignedDistancePlane : public SignedDistancePlane {

protected:

    Vector3 m_point;
    Vector3 m_norm;

public:

    SignedDistancePlane(){}
    virtual void init( Setting& cfg);
    virtual double get_distance( const Vector3& pos) const;

};

REGISTER(SignedDistance,Plane)


void SignedDistancePlane::init( Setting& cfg){
    Vector3 point( 0., 0., 0.);
    Vector3 norm( 0., 0., 0.);
    // Point does not need to be defined
    // Default is [0,0,0]
    try{ point[0] = cfg.lookup("r.x");}
    catch( const std::exception& e){}
    try{ point[1] = cfg.lookup("r.y");}
    catch( const std::exception& e){}
    try{ point[2] = cfg.lookup("r.z");}
    catch( const std::exception& e){}
    // Norm does need to be defined,
    // for norm.x at the very least
    norm[0] = cfg.lookup("n.x");
    try{ norm[1] = cfg.lookup("n.y");}
    catch( const std::exception& e){}
    try{ norm[2] = cfg.lookup("n.z");}
    catch( const std::exception& e){}
    m_point = point;
    m_norm = norm;
}


double SignedDistancePlane::get_distance( const Vector3& pos) const {
    return -1.0 * Geometry::distance_point_plane( pos, m_point, m_norm); // returns dot product
}

}
#endif
