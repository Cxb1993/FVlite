// OperatorInitialiseState.hpp
//
// Writes StateVectors to grid.

#ifndef OPERATORINITIALISESTATE_HPP
#define OPERATORINITIALISESTATE_HPP

#include <string>
#include <libconfig.h++>

#include "OperatorInitialiseBase.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class OperatorInitialiseState : public OperatorInitialiseBase {

protected:

    // Stored as primitives
    MathVector<SIZE> mState;

public:

    OperatorInitialiseState(){}
    virtual ~OperatorInitialiseState(){}

    virtual void init( Setting& cfg);
    virtual void exec( Grid& grid, Timer& timer);
};

// Register with factory

REGISTER( Operator, InitialiseState)

// Function definitions

void OperatorInitialiseState::init( Setting& cfg){
    // Is this inverted? i.e. set signed distance negative
    try{ is_inverted = cfg.lookup("invert"); }
    catch( const std::exception& e){ is_inverted = false;}
    // Set up signed distance function
    mp_signdist = SignedDistanceFactory.create(cfg.lookup("shape").c_str());
    try{
        Setting& paramsCfg = cfg.lookup("params");
        mp_signdist->init( paramsCfg);
    } catch ( const std::exception& e){
        // Ignore cases where params aren't provided. Not all modules need them.
    }
    // Get State
    // set values to a MathVector, not a StateVector
    // This stores the primitive values, which are converted
    // to a conserved form using the set(MathVector) function.
    // For Maxwell's equations, the field itself will be taken.
    Setting& stateCfg = cfg.lookup("state");
    for( int ii=0; ii<SIZE; ii++){
        mState[ii] = stateCfg[ii];
    }
}


void OperatorInitialiseState::exec( Grid& grid, Timer& timer){
    (void)timer;
    // Calculate signed distance at each point.
    // If positive, draw state. Otherwise, do nothing.
    // Grid sweep does not include ghost cells.
    double x, y, distance;
    Vector3 pos;
    Material material;
    unsigned int startX = grid.state_start( DIM_X );
    unsigned int startY = grid.state_start( DIM_Y );
    unsigned int endX = grid.state_end( DIM_X );
    unsigned int endY = grid.state_end( DIM_Y );
    for( unsigned int jj=startY; jj < endY; jj++){
        y = grid.state_position(DIM_Y,jj);
        for( unsigned int ii=startX; ii < endX; ii++){
            x = grid.state_position(DIM_X,ii);
            pos.set( x, y, 0.);
            distance = get_distance(pos);
            material = grid.material(ii,jj);
            if ( (distance >= 0 && !is_inverted) || (distance < 0 && is_inverted) ){
                grid.state(ii,jj) = material.state_from_primitives(mState);
            }
        }
    }
}

}
#endif
