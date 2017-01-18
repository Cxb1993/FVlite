// SignedDistanceTetrahedron.hpp
//
// Returns signed distance from a tetrahedron.
//
// Vector r refers to a corner.
// Vectors b and c define sides of a triangle from that corner
// Vector a makes remaining edge

#ifndef SIGNEDDISTANCETETRAHEDRON_HPP
#define SIGNEDDISTANCETETRAHEDRON_HPP

#include "SignedDistanceAbstract.hpp"

using libconfig::Setting;

namespace FVlite{

class SignedDistanceTetrahedron : public SignedDistance {

protected:

    Vector3 m_r;
    Vector3 m_a;
    Vector3 m_b;
    Vector3 m_c;

public:

    SignedDistanceTetrahedron(){}
    virtual void init( Setting& cfg);
    virtual double get_distance( const Vector3& pos) const;

};

REGISTER(SignedDistance,Tetrahedron)


void SignedDistanceTetrahedron::init( Setting& cfg){
    // There must be an neater way to do this...
    m_r[0] = cfg.lookup("r.x");
    m_r[1] = cfg.lookup("r.y");
    m_r[2] = cfg.lookup("r.z");
    m_a[0] = cfg.lookup("a.x");
    m_a[1] = cfg.lookup("a.y");
    m_a[2] = cfg.lookup("a.z");
    m_b[0] = cfg.lookup("b.x");
    m_b[1] = cfg.lookup("b.y");
    m_b[2] = cfg.lookup("b.z");
    m_c[0] = cfg.lookup("c.x");
    m_c[1] = cfg.lookup("c.y");
    m_c[2] = cfg.lookup("c.z");
}


double SignedDistanceTetrahedron::get_distance( const Vector3& pos) const{
    // is vector a in same direction as normal of bxc?
    bool is_backwards = ( m_a * (m_b^m_c)) < 0;
    MathVector<4> distances;
    distances[0] = distance_point_triangle( pos, m_r, m_b, m_c);
    distances[1] = distance_point_triangle( pos, 
    return levelset;
}

}
#endif
