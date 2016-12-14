// ElectromagneticMaterial.hpp
//
// Describes material parameters. Assumes isotropic.
// Material class is set using relative permittivities and permeabilities.
// It stores the reciprocal of the actual permeabilities, e.g. 1/(eps0*epsRel)
// This minimises the number of dividing operations needed.

#ifndef ELECTROMAGMATERIAL_HPP
#define ELECTROMAGMATERIAL_HPP

#include <cmath>

namespace FVlite{

// Global constants, in SI units
// TODO Find better solution to this?
const double c_eps0 = 8.854187817e-12; // Vacuum permittivity
const double c_mu0  = 1.2566370614e-6; // Vacuum permeability
const double c_c    = 2.99792458e8;    // Vacuum speed of light
const double c_eta0 = 376.73031346177; // Impedance of free space = E/H

class MaterialElectromagnetic {
protected:

    double mEpsRecip; // Electrostatic permittivity reciprocal
    double mMuRecip;  // Magnetic permeability reciprocal
//    double mSigma;    // Conductivity
public:

    MaterialElectromagnetic();
    MaterialElectromagnetic( double epsRel, double muRel);

    double epsilon() const { return 1./mEpsRecip;}
    double mu() const { return 1./mMuRecip;}

    double epsilon_reciprocal() const { return mEpsRecip;}
    double mu_reciprocal() const { return mMuRecip;}

    double epsilon_relative() const { return 1./(mEpsRecip*c_eps0);}
    double mu_relative() const { return 1./(mMuRecip*c_mu0);}

    double local_speed( const StateVector& state);

    void set( double epsilon, double mu);
    void set_relative( double epsRel, double muRel);
    void set_reciprocal( double epsRecip, double muRecip);

    StateVector state_from_primitives( double, double, double, double, double, double) const;
    StateVector state_from_primitives( MathVector<SIZE> prim ) const;
};

MaterialElectromagnetic::MaterialElectromagnetic() : MaterialElectromagnetic(1.,1.){}

MaterialElectromagnetic::MaterialElectromagnetic( double epsRel, double muRel){
    set_relative(epsRel,muRel);
    return;
}


void MaterialElectromagnetic::set( double eps, double mu) {
    mEpsRecip = 1. / eps;
    mMuRecip  = 1. / mu;
}

void MaterialElectromagnetic::set_relative( double epsRel, double muRel){
    mEpsRecip = 1. / (c_eps0 * epsRel);
    mMuRecip  = 1. / (c_mu0 * muRel);
}

void MaterialElectromagnetic::set_reciprocal( double epsRecip, double muRecip){
    mEpsRecip = epsRecip*c_eps0;
    mMuRecip  = muRecip*c_mu0;
}

double MaterialElectromagnetic::local_speed( const StateVector& state){
    // State independent of conserved state vector
    (void)state;
    return sqrt(mEpsRecip*mMuRecip);
}

StateVector MaterialElectromagnetic::state_from_primitives( double Ex, double Ey, double Ez, double Hx, double Hy, double Hz) const{
    // Conservative same as primitive in this case
    StateVector result(Ex,Ey,Ez,Hx,Hy,Hz);
    return result;
}

StateVector MaterialElectromagnetic::state_from_primitives( MathVector<SIZE> prim ) const {
    return state_from_primitives(prim[0],prim[1],prim[2],prim[3],prim[4],prim[5]);
}

} // Namespace closure
#endif /* ELECTROMAGMATERIAL_HPP */
