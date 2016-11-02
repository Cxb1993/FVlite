// OperatorInitialisation.hpp
//
// Operator used by InitialisationManager. Writes to grid
// using user-defined settings.

#ifndef OPERATORINITIALISATION_HPP
#define OPERATORINITIALISATION_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <libconfig.h++>

#include "Grid/Grid.hpp"
#include "Utils/ObjectFactory.hpp"
#include "Initialisation/InitialisationModules.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class OperatorInitialisation : public Operator {

protected:

    bool mSolid;
    bool mInner;
    StateVector mState;

    InitialisationModule* mpInitMod;

public:

    OperatorInitialisation(){}
    virtual ~OperatorInitialisation(){}

    void init( Grid* pGrid, Timer* pTimer, Setting& cfg);
    void exec();
};

// Register with factory

REGISTER( Operator, Initialisation)

// Function definitions

void OperatorInitialisation::init( Grid* pGrid, Timer* pTimer, Setting& cfg){
    Operator::init( pGrid, pTimer, cfg);
    mSolid = cfg.lookup("solid");
    mInner = cfg.lookup("inner");
    if( !mSolid ){
        // set values to a MathVector, not a StateVector
        // This stores the primitive values, which are converted
        // to a conserved form using the set(MathVector) function.
        // For Maxwell's equations, the field itself will be taken.
        Setting& stateCfg = cfg.lookup("state");
        MathVector<SIZE> vector;
        for( int ii=0; ii<SIZE; ii++){
            vector[ii] = stateCfg[ii];
        }
        mState.set(vector);
    }
    mpInitMod = InitialisationModuleFactory.create(cfg.lookup("type").c_str());
    try{
        Setting& paramsCfg = cfg.lookup("params");
        mpInitMod->init( paramsCfg);
    } catch ( const std::exception& e){
        // Ignore cases where params aren't provided. Not all modules need them.
    }
    return;
}


void OperatorInitialisation::exec(){

    // Scan through grid, determine level set function.
    // If fluid:
    //  inner  -> positive level set -> draw mState here
    //  !inner -> negative level set -> draw mState here
    // If solid:
    //  Write level set to workspace.
    //  inner  -> merge()
    //  !inner -> intersect()

    int startX = mpGrid->startX();
    int startY = mpGrid->startY();
    int endX = mpGrid->endX();
    int endY = mpGrid->endY();
    int sizeX = mpGrid->sizeX();
    int sizeY = mpGrid->sizeY();
    double x,y,levelset;

    for( int jj=0; jj<sizeY; jj++){
        y = mpGrid->y(jj);
        for( int ii=0; ii<sizeX; ii++){
            x = mpGrid->x(ii);
            levelset = mpInitMod->exec(x,y);

            if( mSolid){
                mpGrid->levelset()->workspace(ii,jj) = levelset;
            } else {
                if( ii>=startX && ii<endX && jj>=startY && jj<endY){
                    if( (mInner && levelset>0) || (!mInner && levelset<0) ){
                        mpGrid->state(ii,jj) = mState;
                    }
                }
            }
        }
    }

    if(mSolid){
        if(mInner) mpGrid->levelset()->merge();
        else mpGrid->levelset()->intersect();
    }

    return;
}

}// Namespace closure
#endif /* OPERATORINITIALISATION_HPP */
