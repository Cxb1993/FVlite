// OperatorInitialiseMaterial.hpp
//
// Writes Materials to grid.

#ifndef OPERATORINITIALISEMATERIAL_HPP
#define OPERATORINITIALISEMATERIAL_HPP

#include <string>
#include <libconfig.h++>

#include "OperatorInitialiseBase.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class OperatorInitialiseMaterial : public OperatorInitialiseBase {

protected:

    Material m_material;

public:

    OperatorInitialiseMaterial(){}
    virtual ~OperatorInitialiseMaterial(){}

    virtual void init( Setting& cfg);
    virtual void exec( Grid& grid, Timer& timer);
};

// Register with factory

REGISTER( Operator, InitialiseMaterial)

// Function definitions

void OperatorInitialiseMaterial::init( Setting& cfg){
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
    // Get Material
    Setting& matCfg = cfg.lookup("material");
#ifdef MAXWELL
    double epsilon_rel = matCfg.lookup("epsilon");
    double mu_rel = matCfg.lookup("mu");
    m_material.set_relative( epsilon_rel, mu_rel);
#endif
#ifdef EULER
    double gamma = matCfg.lookup("gamma");
    m_material.set(gamma);
#endif
}


void OperatorInitialiseMaterial::exec( Grid& grid, Timer& timer){
    (void)timer;
    // Calculate signed distance at each point.
    // If positive, draw material. Otherwise, do nothing.
    // Grid sweep DOES include ghost cells.
    double x, y, distance;
    Vector3 pos;
    unsigned int startX = 0;
    unsigned int startY = 0;
    unsigned int endX = grid.full_states( DIM_X );
    unsigned int endY = grid.full_states( DIM_Y );
    for( unsigned int jj=startY; jj < endY; jj++){
        y = grid.state_position(DIM_Y,jj);
        for( unsigned int ii=startX; ii < endX; ii++){
            x = grid.state_position(DIM_X,ii);
            pos.set( x, y, 0.);
            distance = get_distance(pos);
            if ( (distance >= 0 && !is_inverted) || (distance < 0 && is_inverted) ){
                grid.material(ii,jj) = m_material;
            }
        }
    }
}

}
#endif
