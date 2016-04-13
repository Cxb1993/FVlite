// Grid.hpp
//
// Stores state vectors, flux vectors and a spatial grid.

#ifndef GRID_HPP
#define GRID_HPP

#include "StateVector.hpp"
#include "FluxVector.hpp"
#include "Material.hpp"

#include <vector>

using std::vector;

namespace FVTD{

class Grid{

private:

    int mNx;             // Number of grid elements in physical domain (not counting boundary cells), x direction
    int mNy;             // Number of grid elements in physical domain (not counting boundary cells), y direction
    int mSizeX;          // Total number of grid elements in x direction
    int mSizeY;          // Total number of grid elements in y direction
    double mLx;          // Physical grid length, x direction
    double mLy;          // Physical grid length, y direction
    double mDx;          // Grid spacing = mLx/mNx
    double mDy;          // Grid spacing = mLy/mNy
    int mBound;          // Number of cells in boundary layer
    int mStartX, mEndX;  // Start and end of real grid (i.e. no boundary cells), x direction
    int mStartY, mEndY;  // Start and end of real grid (i.e. no boundary cells), y direction

    vector<Material>*    pDomain;
    vector<StateVector>* pState;
    vector<StateVector>* pStateAlt; // Secondary storage, needed for predictor/corrector time stepping
    vector<FluxVector>*  pFlux;

public:

    Grid( int Nx, int Ny, double Lx, double Ly, int bound);
    ~Grid();

    // Access functions
    inline Material&    material( int ii, int jj);
    inline StateVector& state( int ii, int jj);
    inline StateVector& stateAlt( int ii, int jj);
    inline FluxVector&  flux( int ii, int jj);

    double dx() const;
    double dy() const;
    double Lx() const;
    double Ly() const;
    int Nx() const;
    int Ny() const;
    int bound() const;
    int startX() const;
    int startY() const;
    int endX() const;
    int endY() const;
    
    void swapStateVectors();

};


Grid::Grid( int Nx, int Ny, double Lx, double Ly, int bound) :
        mNx(Nx), mNy(Ny), mLx(Lx), mLy(Ly), mDx(Lx/Nx), mDy(Ly/Ny), mBound(bound){

    // Determine grid parameters
    mSizeX = Nx + 2*bound;
    mSizeY = Ny + 2*bound;
    mStartX = bound;
    mStartY = bound;
    mEndX = bound + Nx;
    mEndY = bound + Ny;

    // Initialise pointers
    int size  = mSizeX * mSizeY;
    pDomain   = new vector<Material>(size);
    pState    = new vector<StateVector>(size);
    pStateAlt = new vector<StateVector>(size);
    pFlux     = new vector<FluxVector>(size);

}

Grid::~Grid(){
    delete pDomain;
    delete pState;
    delete pStateAlt;
    delete pFlux;
}

Material& Grid::material( int ii, int jj){
    return (*pDomain)[jj*mSizeX+ii];
}

StateVector& Grid::state( int ii, int jj){
    return (*pState)[jj*mSizeX+ii];
}

StateVector& Grid::stateAlt( int ii, int jj){
    return (*pStateAlt)[jj*mSizeX+ii];
}

FluxVector& Grid::flux( int ii, int jj){
    return (*pFlux)[jj*mSizeX+ii];
}

double Grid::dx() const{
    return mDx;
}

double Grid::dy() const{
    return mDy;
}

double Grid::Lx() const{
    return mLx;
}

double Grid::Ly() const{
    return mLx;
}

int Grid::Nx() const{
    return mNx;
}

int Grid::Ny() const{
    return mNy;
}

int Grid::bound() const{
    return mBound;
}

int Grid::startX() const{
    return mStartX;
}

int Grid::startY() const{
    return mStartY;
}

int Grid::endX() const{
    return mEndX;
}

int Grid::endY() const{
    return mEndY;
}

void Grid::swapStateVectors(){
    vector<StateVector>* tmp = pState;
    pState = pStateAlt;
    pStateAlt = tmp;
    return;
}

}// Namespace closure
#endif /* GRID_HPP */
