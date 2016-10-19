// BoundaryGeometry.hpp
//
// Describes geometrical details at cell boundaries

#ifndef BOUNDARYGEOMETRY_HPP
#define BOUNDARYGEOMETRY_HPP

#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>

#include "Vectors/Vector3.hpp"

namespace FVlite{

typedef std::pair<Vector3,Vector3> Slice;

class BoundaryGeometry{

protected:

    double mAlpha;  // Volume of irregular cell relative to that of regular cell
    double mBetaL; // Area of cell left x interface relative to that of regular cell
    double mBetaR; // Area of cell right x interface relative to that of regular cell
    double mBetaB; // Area of cell lower y interface relative to that of regular cell
    double mBetaT; // Area of cell upper y interface relative to that of regular cell

    Vector3 mXb; // Center of boundary interface
    Vector3 mNb; // Normal to boundary interface

public:

    BoundaryGeometry(){}
    BoundaryGeometry( double dx, double dy, double bl, double br, double tl, double tr){ set(dx,dy,bl,br,tl,tr);}

    inline double alpha() const{ return mAlpha;}
    inline double betaL() const{ return mBetaL;}
    inline double betaR() const{ return mBetaR;}
    inline double betaT() const{ return mBetaT;}
    inline double betaB() const{ return mBetaB;}
    inline double& alpha() { return mAlpha;}
    inline double& betaL() { return mBetaL;}
    inline double& betaR() { return mBetaR;}
    inline double& betaT() { return mBetaT;}
    inline double& betaB() { return mBetaB;}
    inline Vector3& Xb(){ return mXb;}
    inline Vector3& Nb(){ return mNb;}

    BoundaryGeometry& operator=( const BoundaryGeometry& other);

    // Set via cell corner level set values
    void set( double dx, double dy, double bl, double br, double tl, double tr);

    // Utilities
    bool isCut();
    static double beta1or0( double beta){ return (beta==0. || beta==1);}

    // Static functions, used to determine boundary parameters
    static double getBeta( double ds, double l, double r); // Determines beta, given left and right levelset
    static Slice getSlice( double dx, double dy, double bL, double bR, double bT, double bB);
    //static double getAlpha( double dx, double dy, double bL, double bR, double bT, double bB); DEPRECATED
    static double getAlpha( double dx, double dy, Slice S, double bl, double br, double tl, double tr);
    static Vector3 getMidpoint( Vector3& P1, Vector3& P2); 
    static Vector3 getNormal2D( Vector3& P1, Vector3& P2); 
};

BoundaryGeometry& BoundaryGeometry::operator=( const BoundaryGeometry& other){
    mAlpha = other.mAlpha;
    mBetaL = other.mBetaL;
    mBetaR = other.mBetaR;
    mBetaT = other.mBetaT;
    mBetaB = other.mBetaB;
    mXb = other.mXb;
    mNb = other.mNb;
    return *this;
}


void BoundaryGeometry::set( double dx, double dy, double bl, double br, double tl, double tr){
    
    // Step 1 : Get betas

    // Left and right
    mBetaL = BoundaryGeometry::getBeta( dy, bl, tl);
    mBetaR = BoundaryGeometry::getBeta( dy, br, tr);
    // Top and bottom
    mBetaT = BoundaryGeometry::getBeta( dx, tl, tr);
    mBetaB = BoundaryGeometry::getBeta( dx, bl, br);

    // Step 1.5: Is this a cut cell?

    if( isCut() ){

        // Step 2: get coordinates of slice

        Slice S;
        S = BoundaryGeometry::getSlice( dx, dy, mBetaL, mBetaR, mBetaT, mBetaB);

        // Step 3 : get Alpha
        mAlpha = BoundaryGeometry::getAlpha( dx, dy, S, bl, br, tl, tr);

        // Step 4 : Get center of boundary interface and normal vector of interface
        
        mXb = BoundaryGeometry::getMidpoint(S.first,S.second);
        mNb = BoundaryGeometry::getNormal2D(S.first,S.second);

        // Check to see if normal is pointing in the right direction.
        
        if( mNb[0] >= 0 ){
            if( mNb[1] > 0){
                if(tr<0) mNb=-mNb;
            } else {
                if(br<0) mNb=-mNb;
            }
        } else {
            if( mNb[1] > 0){
                if(tl<0) mNb=-mNb;
            } else {
                if(bl<0) mNb=-mNb;
            }
        }    
    
    } else {  // Not a cut cell

        // Alt. Step 2: Set alpha to one or zero, depending on betas

        if( mBetaL == 0.){
            mAlpha = 0;
        } else {
            mAlpha = 1;
        }

        // Alt. Step 3: Set vectors to the zero vector

        Vector3 ZEROS;
        ZEROS[0]=0;
        ZEROS[1]=0;
        ZEROS[2]=0;

        mXb = ZEROS;
        mNb = ZEROS;

    }

    return;
}

bool BoundaryGeometry::isCut(){
    if( mBetaL==0. && mBetaR == 0 && mBetaT == 0 && mBetaB == 0 ) return false;
    if( mBetaL==1. && mBetaR == 1 && mBetaT == 1 && mBetaB == 1 ) return false;
    return true;
}

double BoundaryGeometry::getBeta( double ds, double l, double r){

    // Assume level set varies linearly over each edge.
    // Problem reduces to finding the intersection of a line, y=mx+c,  with the y axis.
    //
    // ds     | /
    //        |/
    //        /
    //       /|
    //      / |
    //     /  |
    // 0 ___________
    //     l    r
    //
    // The slope represents the level set function while the y axis represents a boundary.
    // The maximum point, y=ds, can be either dx or dy.
    //
    // First, find the slope, m:
    // m = (ds-0)/(r-l)
    //
    // The intercept, c, follows as:
    // c  = y  -  m*x
    //    = 0  -  m*(r)
    // or = dy -  m*(l)
    //
    // The length exposed is then given as c, or ds-c
    // To determine which, consider the gradient of the line.
    // If m>0, c
    // If m<0, ds-c
    // Hence, beta = (ds-c)/ds or c/ds
    
    double m, c, beta;

    if( l * r >= 0){ // No sign change
        if( l < 0 || r < 0){ // If completely outside material
            beta = 1;
        } else {
            beta = 0;
        }
    } else { 
        m = ds/(r-l);
        c = -m*l;
        if( m >= 0){
            beta = c/ds;
        } else {
            beta = (ds-c)/ds;
        }
    }

    return beta;
}

Slice BoundaryGeometry::getSlice( double dx, double dy, double bL, double bR, double bT, double bB){

    // Given betas, determine coordinates that define the 'slice' through the cell.
    // This is given by the points on the two edges that are cut.
    // Care should be taken when the slice cuts through a vertex!
    // EDIT: fixed by imposing small tolerance on level set. Now it can't be zero on a vertex.
    // The slice coordinates are given with respect to the bottom-left vertex.

    Vector3 P1, P2;

    // Working in 2D. Avoid unintialised data.
    P1[2] = 0;
    P2[2] = 0;

    Vector3* P = &P1; // Use this pointer to set, switch it after one point is filled.
    int numset = 0; // count number of points determined

    // Check each boundary in turn

    // LEFT
    if( numset < 2){
        if( !BoundaryGeometry::beta1or0(bL) ){
            (*P)[0] = 0.;
            if( bT < bB ) (*P)[1] = bL*dy; else (*P)[1] = (1.-bL)*dy;
            P = &P2;
            numset++;
        }
    }

    // RIGHT
    if( numset < 2){
        if( !BoundaryGeometry::beta1or0(bR) ){
            (*P)[0] = dx;
            if( bT < bB ) (*P)[1] = bR*dy; else (*P)[1] = (1.-bR)*dy;
            P = &P2;
            numset++;
        }
    }

    // BOTTOM
    if( numset < 2){
        if( !BoundaryGeometry::beta1or0(bB) ){
            (*P)[1] = 0.;
            if( bR < bL ) (*P)[0] = bB*dx; else (*P)[0] = (1.-bB)*dx;
            P = &P2;
            numset++;
        }
    }

    // TOP
    if( numset < 2){
        if( !BoundaryGeometry::beta1or0(bT) ){
            (*P)[1] = dy;
            if( bR < bL ) (*P)[0] = bT*dx; else (*P)[0] = (1.-bT)*dx;
            P = &P2;
            numset++;
        }
    }

    if( numset != 2 || P != &P2 ){
        std::cerr << "\n\nError: BoundaryGeometry::getSlice did something crazy\n" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Bundle vectors together and return
    Slice S(P1,P2); 
    return S;
}

double BoundaryGeometry::getAlpha( double dx, double dy, Slice S, double bl, double br, double tl, double tr){

    // Assume boundary is a straight line cutting the cell.
    // Determine if problem can be resolved as a triangle or a trapezium.
    // i.e. The cell may be cut in two different ways:
    // 
    //  Cut adjacent edges      Cut opposite edges
    //  ________________        ________________
    //  |              |        |             _|  
    //  |             /|        |          __/ |  
    //  |            / |        |       __/    |  
    //  |           /  |        |    __/       |  
    //  |          /   |        | __/          |  
    //  |         /    |        |/             |
    //  _________/______        ________________
    //     'Triangle'              'Trapezium'
    

    //  First, test the case that there isn't a boundary!

    if( bl < 0. && br < 0. && tl < 0. && tr < 0.) return 1.;
    if( bl > 0. && br > 0. && tl > 0. && tr > 0.) return 0.;

    // Otherwise, start calculating

    double alpha=10; // Value assigned arbitrarily to avoid 'uninitialised variable' errors

    // Setup, determine parameters of the polygon defined by positive level set.
    // The two coordinates of the cut are the first two vertices.
    // Any cell vertices with a positive level set are also added.

    std::vector<Vector3> Vertices;
    Vertices.clear(); // Ensure vector size=0. Might not be needed.

    Vertices.push_back(S.first);
    Vertices.push_back(S.second);
    int n_vertices = 2;

    Vector3 P; P[2] = 0;
    if( bl>0.){
        P[0] = 0;
        P[1] = 0;
        Vertices.push_back(P);
        n_vertices++;
    }
    if( br>0.){
        P[0] = dx;
        P[1] = 0;
        Vertices.push_back(P);
        n_vertices++;
    }
    if( tl>0.){
        P[0] = 0;
        P[1] = dy;
        Vertices.push_back(P);
        n_vertices++;
    }
    if( tr>0.){
        P[0] = dx;
        P[1] = dy;
        Vertices.push_back(P);
        n_vertices++;
    }

    // Determine problem type from number of vertices. Solve alpha accordingly.

    double dA = dx*dy;
    double triangle_area = 0.5*(fabs(Vertices[0][0]-Vertices[1][0])*fabs(Vertices[0][1]-Vertices[1][1]));
    //std::cerr << "triangle_area: " << triangle_area/dA << std::endl;
    //std::cerr << "n_Vertices: " << n_vertices << std::endl;
    //std::cerr << "Slice1: (" << S.first[0]/dx << "," <<  S.first[1]/dy << ")" << std::endl;
    //std::cerr << "Slice2: (" << S.second[0]/dx << "," <<  S.second[1]/dy << ")" << std::endl;
    //std::cerr << "levelset bl,br,tl,tr: " << bl << " " << br << " " << tl << " " << tr <<std::endl;

    // Triangular problem
    if( n_vertices == 3  || n_vertices == 5 ){
        if( n_vertices==3){
            alpha = (dA-triangle_area)/dA;
        } else {
            alpha = triangle_area/dA;
        }
    }

    // Trapezium problem
    if( n_vertices == 4 ){
        double rectangle_area;
        // find whether slice is horizontal or vertical.
        // Find area of rectangle accordingly.
        if( Vertices[0][0] == 0. || Vertices[1][0] == 0. ){ // horizontal
            if( Vertices[2][1] == 0 ){ // bottom side solid
                rectangle_area = dx*fmin(Vertices[0][1],Vertices[1][1]);
            } else { // top side solid
                rectangle_area = dx*(dy-fmax(Vertices[0][1],Vertices[1][1]));
            }
        } else { // vertical
            if( Vertices[2][0] == 0 ){ // left side solid
                rectangle_area = dy*fmin(Vertices[0][0],Vertices[1][0]);
            } else { // right side solid
                rectangle_area = dy*(dx-fmax(Vertices[0][0],Vertices[1][0]));
            }
        }
        alpha = (dA - rectangle_area - triangle_area) / dA;
    }

    if( n_vertices < 3 || n_vertices > 5){
        std::cerr << "\n\nError, alpha calculation did something funky.\n" << std::endl;
        exit(EXIT_FAILURE);
    }

    return alpha;
}

// DEPRECATED
/*

double BoundaryGeometry::getAlpha( double dx, double dy, double bL, double bR, double bT, double bB){

    // Assume boundary is a straight line cutting the cell.
    // Determine if problem can be resolved as a triangle or a trapezium.
    // i.e. The cell may be cut in two different ways:
    // 
    //  Cut adjacent edges      Cut opposite edges
    //  ________________        ________________
    //  |              |        |             _|  
    //  |             /|        |          __/ |  
    //  |            / |        |       __/    |  
    //  |           /  |        |    __/       |  
    //  |          /   |        | __/          |  
    //  |         /    |        |/             |
    //  _________/______        ________________
    //     'Triangle'              'Trapezium'
    //
    //  First, test the case that there isn't a boundary!


    if( bL == 1. && bR == 1. && bT == 1. && bB == 1.) return 1.;
    if( bL == 0. && bR == 0. && bT == 0. && bB == 0.) return 0.;

    // Setup, determine cut type.
    // The variable 'filled' is true if most of the cell is solid, false otherwise.

    double alpha;

    bool triangle=false;
    bool trapezium=false;
    bool filled=false;
    double length1=0.;
    double length2=0.;
    double ds=0.;

    if( (bL==1. && bT == 1.) || (bL==0. && bT==0.) ){
        length1 = bB*dx;
        length2 = bR*dy;
        triangle = true;
        if(bL==0.) filled = true;
    }

    if( (bL==1. && bB == 1.) || (bL==0. && bB==0.) ){
        length1 = bT*dx;
        length2 = bR*dy;
        triangle = true;
        if(bL==0.) filled = true;
    }

    if( (bR==1. && bT == 1.) || (bR==0. && bT==0.) ){
        length1 = bB*dx;
        length2 = bL*dy;
        triangle = true;
        if(bR==0.) filled = true;
    }

    if( (bR==1. && bB == 1.) || (bR==0. && bB==0.) ){
        length1 = bT*dx;
        length2 = bL*dy;
        triangle =true;
        if(bR==0.) filled = true;
    }

    // Determine alpha. Method varies depending on cut type.
    double dA = dx*dy;
    double triangle_area;
    double trapezium_area;

    if( triangle ){

        // If triangle:
        //  > Determine area of triangle, as fraction of cell volume
        //  > Determine whether the cell is mostly solid or mostly fluid/free space.
        //  > Add or subtract as appropriate


        if( filled ){
            triangle_area = 0.5*(length1*length2);
            alpha =  triangle_area / dA;
        } else {
            length1 = dx-length1;
            length2 = dy-length2;
            triangle_area = 0.5*(length1*length2);
            alpha = (dA-triangle_area) / dA;
        }

    } else { // trapezium

        // If trapezium:
        //  > Determine which side of cell is fluid/free space, which is solid.
        //  > Determine trapezium area.
        //  > Determine fraction of cell area taken up by trapezium.

        if( (bL==0. && bR==1.) || (bL==1. && bR==0.) ){ // Vertical cut
            length1 = bT*dx;
            length2 = bB*dx;
            ds = dy;
            trapezium = true;
        }

        if( (bT==0. && bB==1.) || (bT==1. && bB==0.) ){ // Horizontal cut
            length1 = bL*dy;
            length2 = bR*dy;
            ds = dx;
            trapezium = true;
        }

        // Error check
        if( !trapezium ){ // If we get here without triangle or trapzium, something went wrong!
            std::cerr << "ERROR, getAlpha, boundary error." << std::endl;
    //        exit(EXIT_FAILURE);
        }

        trapezium_area = ds*0.5*(length1+length2);
        alpha = trapezium_area / dA;

    }

    return alpha;
}
*/

Vector3 BoundaryGeometry::getMidpoint( Vector3& P1, Vector3& P2){
    Vector3 result;
    result = 0.5*(P1+P2);
    return result;
}

Vector3 BoundaryGeometry::getNormal2D( Vector3& P1, Vector3& P2){
    Vector3 P1toP2, result;
    P1toP2 = P2 - P1;
    // Get negative inverse
    // Use P1toP2[2] as temp storage
    result[0] = P1toP2[1];
    result[1] = -P1toP2[0];
    result[2] = 0;
    // normalise
    result = result/result.norm();
    return result;
}

} // Namespace closure
#endif /* BOUNDARYGEOMETRY_HPP */
