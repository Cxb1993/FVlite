// Geometry.hpp
//
// Collection of geometrical functions for general use

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <cmath>
#include <array>
#include <algorithm>

namespace FVlite{
namespace Geometry{

// Typedeffing MathVector<3> rather than using Vector3.
// This is because arithmetic between Vector3's results in MathVector<3>.
// As a result, the following code would fail:
//
// double distance_point_point( const Vector3& r1, const Vector3& r2){...}
// x = distance_point_point( a-b, c);
//
// This is because a-b is NOT of type Vector3.

typedef MathVector<3> Vector;

// Distance between two points
double distance_point_point( const Vector& x1, const Vector& x2){
    return norm(x1-x2);
}

// Distance between point x and an infinite line defined by point r and direction t
double distance_point_line( const Vector& x, const Vector& r, const Vector& t){
    return fabs(norm( curl(x-r,t) / t.norm()));
}

// Distance between point x and a plane defined by point r and normal n
// Note: Carries a sign to indicate which side of the plane x is on.
//       Positive if normal points towards x.
double distance_point_plane( const Vector& x, const Vector& r, const Vector& n){
    return dot(x-r,n) / n.norm();
}

// Projection of point x onto a plane defined by point r and normal n
Vector project_point_plane( const Vector& x, const Vector& r, const Vector& n){
    return x - ( n * distance_point_plane(x,r,n)) / n.norm();
}

// Distance between point x and a line segment defined by point r and directional length t
double distance_point_linesegment( const Vector& x, const Vector& r, const Vector& t){
    double distance;
    // On either side of the line segment?
    // If so, distance between x and r OR x and r+t
    // If not, standard distance from line
    if ( distance_point_plane( x, r, t) <= 0 ){
        distance = distance_point_point( x, r);
    } else if ( distance_point_plane( x, r+t, t) >= 0 ){
        distance = distance_point_point( x, r+t);
    } else {
        distance = distance_point_line( x, r, t);
    }
    return distance;
}

// Signed distance between a point x and a triangle defined by corner A and sides b and c
// Sign given by norm of plane containing the triangle, in direction |a^b|
// Positive if norm points towards x.
double distance_point_triangle( const Vector& x,
                                const Vector& A,
                                const Vector& b,
                                const Vector& c ){
    // Project point onto triangle. If within points, return distance from point to plane.
    // Otherwise, minimum distance from each line segment.
    double distance;
    Vector norm = curl(b,c);
    Vector B = A+b;
    Vector C = A+c;
    Vector P = project_point_plane(x,A,norm);
    // Is this inside triangle?
    // if PAxPB, PBxPC, PCxPA all point in same direction, inside.
    // otherwise, outside
    bool PAxPBsign = std::signbit( dot(curl(P-A,P-B), norm));
    bool PBxPCsign = std::signbit( dot(curl(P-B,P-C), norm));
    bool PCxPAsign = std::signbit( dot(curl(P-C,P-A), norm));
    if( (PAxPBsign && PBxPCsign && PCxPAsign) || (!PAxPBsign && !PBxPCsign && !PCxPAsign)){
        distance = distance_point_plane( x, A, norm);
    } else {
        double d0 = distance_point_linesegment( x, A, b);
        double d1 = distance_point_linesegment( x, A, c);
        double d2 = distance_point_linesegment( x, B, C-B);
        distance = ( d0 < d1 )? d0 : d1;
        distance = ( d2 < distance) ? d2 : distance;
        if( std::signbit(distance_point_plane( x, A, norm)) ) distance *= -1;
    }
    return distance;
}



}}
#endif
