// BoundaryModuleAbstract.hpp
//
// Abstract boundary module.

#ifndef INITIALISATIONMODULEABSTRACT_HPP
#define INITIALISATIONMODULEABSTRACT_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <libconfig.h++>

#include "Grid/Grid.hpp"
#include "Utils/ObjectFactory.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{


class InitialisationModule{

protected:

    Grid* pGrid;

    bool mSolid;
    bool mInner;
    int mRank;
    StateVector mState;

public:

    InitialisationModule(){}
    InitialisationModule( const InitialisationModule& other);
    virtual ~InitialisationModule(){}

    void init( Grid* pGrid, Setting& cfg);
    virtual void init_params( Setting& cfg) = 0;
    void init_state(Setting& cfg);

    void exec();
    virtual double getLevelSet( double x, double y) = 0;
    int rank(){ return mRank;}

};

// Setting up factory

ObjectFactory<InitialisationModule> InitialisationModuleFactory;

// Function definitions

void InitialisationModule::init( Grid* pGrid, Setting& cfg){
    (*this).pGrid = pGrid;
    mSolid = cfg.lookup("solid");
    mInner = cfg.lookup("inner");
    mRank  = cfg.lookup("rank");
    Setting& paramsCfg = cfg.lookup("params");
    init_params( paramsCfg);
    if( !mSolid ){
        Setting& stateCfg = paramsCfg.lookup("state");
        init_state(stateCfg);
    }
    return;
}

void InitialisationModule::init_state( Setting& cfg){
    // set values to a MathVector, not a StateVector
    // This stores the primitive values, which are converted
    // to a conserved form using the set(MathVector) function.
    MathVector<SIZE> vector;
    for( int ii=0; ii<SIZE; ii++){
        vector[ii] = cfg[ii];
    }
    mState.set(vector);
    return;
}

InitialisationModule::InitialisationModule( const InitialisationModule& other) 
    : pGrid(other.pGrid), mSolid(other.mSolid), mRank(other.mRank), mState(other.mState)
{}

void InitialisationModule::exec(){

    // Scan through grid, determine level set function.
    // If fluid:
    //  inner  -> positive level set -> draw mState here
    //  !inner -> negative level set -> draw mState here
    // If solid:
    //  Write level set to workspace.
    //  inner  -> merge()
    //  !inner -> intersect()

    int startX = pGrid->startX();
    int startY = pGrid->startY();
    int endX = pGrid->endX();
    int endY = pGrid->endY();
    int sizeX = pGrid->sizeX();
    int sizeY = pGrid->sizeY();
    double x,y,levelset;

    for( int jj=0; jj<sizeY; jj++){
        y = pGrid->y(jj);
        for( int ii=0; ii<sizeX; ii++){
            x = pGrid->x(ii);
            levelset = getLevelSet(x,y);

            if( mSolid){
                pGrid->levelset()->workspace(ii,jj) = levelset;
            } else {
                if( ii>=startX && ii<endX && jj>=startY && jj<endY){
                    if( (mInner && levelset>0) || (!mInner && levelset<0) ){
                        pGrid->state(ii,jj) = mState;
                    }
                }
            }
        }
    }

    if(mSolid){
        if(mInner) pGrid->levelset()->merge();
        else pGrid->levelset()->intersect();
    }

    return;
}

}// Namespace closure
#endif /* INITIALISATIONMODULEABSTRACT_HPP */
