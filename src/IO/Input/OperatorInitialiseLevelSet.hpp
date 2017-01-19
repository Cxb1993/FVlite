// OperatorInitialiseLevelSet.hpp
//
// Writes signed distance directly to grid.

#ifndef OPERATORINITIALISELEVELSET_HPP
#define OPERATORINITIALISELEVELSET_HPP

#include <string>
#include <libconfig.h++>

#include "OperatorInitialiseBase.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class OperatorInitialiseLevelSet : public OperatorInitialiseBase {
public:

    OperatorInitialiseLevelSet(){}
    virtual ~OperatorInitialiseLevelSet(){}

    virtual void init( Setting& cfg);
    virtual void exec( Grid& grid, Timer& timer);
};

// Register with factory

REGISTER( Operator, InitialiseLevelSet)

// Function definitions

void OperatorInitialiseLevelSet::init( Setting& cfg){
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
}


void OperatorInitialiseLevelSet::exec( Grid& grid, Timer& timer){
    (void)timer;
    // Calculate signed distance at each point.
    // Write to LevelSet Workspace.
    // Merge with true LevelSet.
    // Calculates at all vertices
    double x, y, distance;
    Vector3 pos;
    unsigned int startX = 0;
    unsigned int startY = 0;
    unsigned int endX = grid.full_levelsets( DIM_X );
    unsigned int endY = grid.full_levelsets( DIM_Y );
    grid.reset_workspace();
    for( unsigned int jj=startY; jj < endY; jj++){
        y = grid.levelset_position(DIM_Y,jj);
        for( unsigned int ii=startX; ii < endX; ii++){
            x = grid.levelset_position(DIM_X,ii);
            pos.set( x, y, 0.);
            distance = get_distance(pos);
            distance *= ( is_inverted) ? -1 : 1;
            grid.workspace(ii,jj) = distance;
        }
    }
    grid.merge_levelset();
}

}
#endif
