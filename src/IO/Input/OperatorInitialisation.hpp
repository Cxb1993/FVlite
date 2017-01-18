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
    Material mMat;

    InitialisationModule* mpInitMod;

public:

    OperatorInitialisation(){}
    virtual ~OperatorInitialisation(){}

    void init( Setting& cfg);
    void exec( Grid& grid, Timer& timer);
};

// Register with factory

REGISTER( Operator, Initialisation)

// Function definitions

void OperatorInitialisation::init( Setting& cfg){
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
        mState = mMat.state_from_primitives(vector);
        // TODO Get rid of awful #ifdef nonsense
        Setting& matCfg = cfg.lookup("material");
#ifdef MAXWELL
        double epsilon_rel = matCfg.lookup("epsilon");
        double mu_rel = matCfg.lookup("mu");
        mMat.set_relative( epsilon_rel, mu_rel);
#endif
#ifdef EULER
        double gamma = matCfg.lookup("gamma");
        mMat.set(gamma);
#endif
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


void OperatorInitialisation::exec( Grid& grid, Timer& timer){
    (void)timer;

    // Build level set workspace
    // Sweep over all points, including ghost cells

    grid.reset_workspace();
    double x,y,levelset;

    for( unsigned int jj=0; jj < grid.full_levelsets(DIM_Y); jj++){
        y = grid.levelset_position(DIM_Y,jj);
        for( unsigned int ii=0; ii < grid.full_levelsets(DIM_X); ii++){
            x = grid.levelset_position(DIM_X,ii);
            levelset = mpInitMod->exec(x,y);
            grid.workspace(ii,jj) = levelset;
        }
    }

    // If intending to write states and materials, use this
    // to calculate approximate central levelsets
    if( !mSolid ){
        unsigned int startX = grid.state_start(DIM_X);
        unsigned int startY = grid.state_start(DIM_Y);
        unsigned int endX = grid.state_end(DIM_X);
        unsigned int endY = grid.state_end(DIM_Y);
        unsigned int ls_startX = grid.levelset_start(DIM_X);
        unsigned int ls_startY = grid.levelset_start(DIM_Y);
        for( unsigned int jj = startY, jjL = ls_startY; jj<endY; jj++, jjL++){
            for( unsigned int ii = startX, iiL = ls_startX; ii<endX; ii++, iiL++){
                levelset = grid.approx_central_workspace(iiL,jjL);
                if( (mInner && levelset>0) || (!mInner && levelset<0) ){
                    grid.state(ii,jj) = mState;
                    grid.material(ii,jj) = mMat;
                }
            }
        }
    }

    // If intending to write a 'solid' boundary, merge workspace and levelset
    if( mSolid ){
        if(mInner) grid.merge_levelset();
        //else grid.intersect_levelset() // NOTE: CURRENTLY BROKEN
    }

    return;
}

}// Namespace closure
#endif /* OPERATORINITIALISATION_HPP */
