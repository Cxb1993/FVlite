// SignedDistanceBackground.hpp
//
// Used to set background. Returns positive constant regardless of grid position.
// Not recommended to couple this with the 'solid' or 'outer' settings!
// Suitable for setting StateVectors/Fields/etc on the grid, but for little else.
// Should always be called first, as otherwise it will overwrite anything else
// drawn on the grid.

#ifndef SIGNEDDISTANCEBACKGROUND_HPP
#define SIGNEDDISTANCEBACKGROUND_HPP

#include "SignedDistanceAbstract.hpp"

using libconfig::Setting;

namespace FVlite{

class SignedDistanceBackground : public SignedDistance {

public:

    SignedDistanceBackground(){}
    virtual void init( Setting& cfg);
    virtual double get_distance( const Vector3& pos) const;

};

REGISTER(SignedDistance,Background)


void SignedDistanceBackground::init( Setting& cfg){
    // Class holds no state, nothing to configure
    (void)cfg;
}


double SignedDistanceBackground::get_distance( const Vector3& pos) const{
    // Position in grid is irrelevant
    // Return 1.
    (void)pos;
    return 1.;
}

}
#endif 
