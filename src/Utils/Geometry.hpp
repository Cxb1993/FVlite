// Geometry.hpp
//
// Collection of geometrical functions for general use

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <cmath>
#include <array>
#include <algorithm>

#include "Vectors/Vector3.hpp"

namespace FVlite{
namespace Geometry{

// Distance between two points
double distance_point_point( const Vector3& x1, const Vector3& x2){
    Vector3 vec_dist = r1 - r2;
    return vec_dist.norm;
}

// Distance between point x and an infinite line defined by point r and direction t
double distance_point_line( const Vector3& x, const Vector3& r, const Vector3& t){
    return fabs( (x-r)^t / t.norm() );
}

// Distance between point x and a plane defined by point r and normal n
// Note: Carries a sign to indicate which side of the plane x is on.
//       Positive if normal points towards x.
double distance_point_plane( const Vector3& x, const Vector3& r, const Vector3& n){
    return (x-r) * n / n.norm();
}

// Projection of point x onto a plane defined by point r and normal n
Vector3 project_point_plane( const Vector3& x, const Vector3& r, const Vector3& n){
    Vector3 norm = n/n.norm();
    Vector3 result = x - ( n * distance_point_plane(x,r,n));
    return result;
}

// Distance between point x and a line segment defined by point r and directional length t
double distance_point_linesegment( const Vector3& x, const Vector3& r, const Vector3& t){
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
double distance_point_triangle( const Vector3& x,
                                const Vector3& A,
                                const Vector3& b,
                                const Vector3& c ){
    // Project point onto triangle. If within points, return distance from point to plane.
    // Otherwise, minimum distance from each line segment.
    double distance;
    Vector3 norm = b^c;
    Vector3 B = A+b;
    Vector3 C = A+c;
    Vector3 P = project_point_plane(x,A,norm);
    // Is this inside triangle?
    // if PAxPB, PBxPC, PCxPA all point in same direction, inside.
    // otherwise, outside
    bool PAxPBsign = signbit(((p-a)^(p-b)) * norm);
    bool PBxPCsign = signbit(((p-b)^(p-c)) * norm);
    bool PCxPAsign = signbit(((p-c)^(p-a)) * norm);
    if( (PAxPBsign && PBxPCsign && PCxPAsign) || (!PAxPBsign && !PBxPCsign && !PCxPAsign)){
        distance = distance_point_plane( x, A, bxc);
    } else {
        double d0 = distance_point_linesegment( x, A, b);
        double d1 = distance_point_linesegment( x, A, c);
        double d2 = distance_point_linesegment( x, B, C-B);
        distance = ( d0 < d1 )? d0 : d1;
        distance = ( d2 < distance) ? d2 : distance;
        if( signbit(distance_point_plane(x,A,bxc)) ) distance *= -1;
    }
    return distance;
}



}}
#endif
