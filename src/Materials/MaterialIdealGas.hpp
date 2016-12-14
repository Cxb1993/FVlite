// MaterialIdealGas.hpp
//
// Describes material parameters. Assumes constant.

#ifndef IDEALGASMATERIAL_HPP
#define IDEALGASMATERIAL_HPP

#include <cmath>

namespace FVlite{

class MaterialIdealGas {
protected:

    double mGamma; // Ratio of specific heat capacities

public:

    MaterialIdealGas();
    MaterialIdealGas( double gamma);

    double gamma() const { return mGamma;}
    void set( double gamma){ mGamma = gamma;}

    double local_speed( const StateVector& state) const;
    Vector3 local_velocity( const StateVector& state) const;
    double sound_speed( const StateVector& state) const;
    double internal_energy( const StateVector& state) const;
    double pressure( const StateVector& state) const;

    StateVector state_from_primitives( double rho, double ux, double uy, double p) const;
    StateVector state_from_primitives( MathVector<SIZE> ) const;
};

MaterialIdealGas::MaterialIdealGas() : MaterialIdealGas(1.4){}

MaterialIdealGas::MaterialIdealGas( double gamma){
    set(gamma);
}

double MaterialIdealGas::local_speed( const StateVector& state) const {
    double max_local_speed = fabs(state.ux());
    max_local_speed = (fabs(state.uy()) > max_local_speed) ? fabs(state.uy()) : max_local_speed; 
    double local_sound_speed = sound_speed(state);
    double total_local_speed = max_local_speed + local_sound_speed;
    if( fabs(total_local_speed) > 3e8 ){
        std::cerr << "ERROR: SOUND SPEED BROKEN SPEED OF LIGHT!" << std::endl;
        exit(EXIT_FAILURE);
    }
    return total_local_speed;
}

Vector3 MaterialIdealGas::local_velocity( const StateVector& state) const {
    Vector3 result;
    result[0] = state.ux();
    result[1] = state.uy();
    result[2] = 0.;
    return result;
}

double MaterialIdealGas::sound_speed( const StateVector& state) const {
    return sqrt( mGamma * pressure(state)/state.rho());
}

double MaterialIdealGas::internal_energy( const StateVector& state) const {
    return pressure(state)/(state.rho()*(mGamma-1.));
}

double MaterialIdealGas::pressure( const StateVector& state) const {
    return (mGamma-1.)*( state.E() - 
            0.5*state.rho()*( state.ux()*state.ux() + state.uy()*state.uy()));
}

StateVector MaterialIdealGas::state_from_primitives( double rho, double ux, double uy, double p) const{
    StateVector result;
    result[0] = rho;
    result[1] = rho*ux;
    result[2] = rho*uy;
    result[3] = 0.5*rho*(ux*ux+uy*uy) + p/(mGamma-1.);
    return result;
}

StateVector MaterialIdealGas::state_from_primitives( MathVector<SIZE> primitives) const{
    return state_from_primitives(primitives[0],primitives[1],primitives[2],primitives[3]);
}

} // Namespace closure
#endif /* IDEALGASMATERIAL_HPP */
