// Grid.hpp
//
// Stores state vectors, flux vectors and a spatial grid.

#ifndef GRID_HPP
#define GRID_HPP

#include "Vectors/StateVector.hpp"
#include "Vectors/FluxVector.hpp"
#include "BoundaryGeometry.hpp"
#include "LevelSet.hpp"

#include <vector>
#include <libconfig.h++>

using std::vector;
using libconfig::Setting;

namespace FVlite{

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

    vector<StateVector>* pState;
    vector<FluxVector>*  pFlux;

    vector<StateVector>* pStateAux; // Auxiliary state, used for cut cell method
    vector<FluxVector>*  pFluxAux;  // Auxiliary flux, used for cut cell method
    vector<BoundaryGeometry>* pGeometry; // Contains geometry information for cut cell method

    LevelSet* pLevelSet; // Contains level set function

public:

    Grid(){}
    Grid( Setting& cfg){ init(cfg);}
    ~Grid();

    void init( Setting& cfg);

    // Access functions
    inline StateVector& state( int ii, int jj){ return (*pState)[jj*mSizeX+ii];}
    inline StateVector& state_aux( int ii, int jj){ return (*pStateAux)[jj*mSizeX+ii];}
    inline FluxVector&  flux( int ii, int jj){ return (*pFlux)[jj*mSizeX+ii];}
    inline FluxVector&  flux_aux( int ii, int jj){ return (*pFluxAux)[jj*mSizeX+ii];}
    inline BoundaryGeometry& boundary( int ii, int jj){ return (*pGeometry)[jj*mSizeX+ii];}
    
    inline LevelSet* levelset(){ return pLevelSet;}
    inline double levelset(int ii,int jj) const{ return (*pLevelSet)(ii,jj);}
    inline double levelset_workspace(int ii,int jj) const{ return pLevelSet->workspace(ii,jj);}
    inline double& levelset(int ii,int jj){ return (*pLevelSet)(ii,jj);}
    inline double& levelset_workspace(int ii,int jj){ return levelset()->workspace(ii,jj);}

    // Cell center locations
    double x( int ii) const{ return (ii-mBound+0.5)*dx();}
    double y( int jj) const{ return (jj-mBound+0.5)*dy();}

    double dx() const { return mDx;}
    double dy() const { return mDy;}
    double Lx() const { return mLx;}
    double Ly() const { return mLy;}
    int Nx() const { return mNx;}
    int Ny() const { return mNy;}
    int bound() const { return mBound;}
    int startX() const { return mStartX;}
    int startY() const { return mStartY;}
    int endX() const { return mEndX;}
    int endY() const { return mEndY;}
    int sizeX() const { return mSizeX;}
    int sizeY() const { return mSizeY;}

    // Get max speed
    double maxSpeed();
};

// Function definitions

void Grid::init( Setting& cfg){

    mNx = cfg.lookup("cells.x");
    mNy = cfg.lookup("cells.y");
    mLx = cfg.lookup("size.x");
    mLy = cfg.lookup("size.y");

    // Boundary size, hardcoded
    mBound = 2;

    // Determine grid parameters
    mDx = mLx/mNx;
    mDy = mLy/mNy;
    mSizeX = mNx + 2*mBound;
    mSizeY = mNy + 2*mBound;
    mStartX = mBound;
    mStartY = mBound;
    mEndX = mBound + mNx;
    mEndY = mBound + mNy;

    // Initialise pointers
    int size  = mSizeX * mSizeY;
    pState    = new vector<StateVector>(size);
    pStateAux = new vector<StateVector>(size);
    pFlux     = new vector<FluxVector>(size);
    pFluxAux  = new vector<FluxVector>(size);
    pGeometry = new vector<BoundaryGeometry>(size);

    // Initialise LevelSet
    pLevelSet = new LevelSet(mNx,mNy,mBound,mDx,mDy,mLx,mLy);
    pLevelSet->init();

    return;
}

Grid::~Grid(){
    delete pState;
    delete pFlux;
    delete pStateAux;
    delete pFluxAux;
    delete pGeometry;
    delete pLevelSet;
}

double Grid::maxSpeed(){
    double max = 0.;
    double local_sound_speed;
    double max_local_speed;
    double total_local_speed;
#ifdef MAXWELL
    (void)local_sound_speed;
    (void)max_local_speed;
    (void)total_local_speed;
    max = c_c; // Speed of light
#else
    for( int jj=mStartY; jj<mEndY; jj++){
        for( int ii=mStartX; ii<mEndX; ii++){
            max_local_speed = fabs(state(ii,jj).ux());
            max_local_speed = (fabs(state(ii,jj).uy()) > max_local_speed) ? fabs(state(ii,jj).uy()) : max_local_speed; 
            local_sound_speed = state(ii,jj).a();
            total_local_speed = max_local_speed + local_sound_speed;
            if( fabs(total_local_speed) > 3e8 ){
                std::cerr << "ERROR: SOUND SPEED BROKEN SPEED OF LIGHT! GRID CELL: (" << ii << "," <<jj<<")"<<std::endl;
                exit(EXIT_FAILURE);
            }
            max = (total_local_speed > max) ? total_local_speed : max;
        }
    }
#endif
    return max;
}

}// Namespace closure
#endif /* GRID_HPP */
