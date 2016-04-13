// Material.hpp
//
// Describes material parameters
// Material class is set using relative permittivities and permeabilities.
// It stores the reciprocal of the actual permeabilities, e.g. 1/(eps0*epsRel)
// This minimises the number of dividing operations needed.

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <cmath>

namespace FVTD{

// Global constants, in SI units
const double c_eps0 = 8.854187817e-12; // Vacuum permittivity
const double c_mu0  = 1.2566370614e-6; // Vacuum permeability
const double c_c    = 2.99792458e8;    // Vacuum speed of light
const double c_eta0 = 376.73031346177; // Impedance of free space = E/H

class Material{
protected:

    double mEpsRecip; // Electrostatic permittivity reciprocal
    double mMuRecip;  // Magnetic permeability reciprocal
    //double mSigma;  // Conductivity (coming soon!)

public:

    Material();
    Material( double epsRel, double muRel);

    inline double epsRecip() const{ return mEpsRecip;}
    inline double muRecip() const{ return mMuRecip;}

    inline double epsRel() const{ return 1./(mEpsRecip*c_eps0);}
    inline double muRel() const{ return 1./(mMuRecip*c_mu0);}

    inline void set( double epsRel, double muRel);
    inline double speed();
};

Material::Material(){}

Material::Material( double epsRel, double muRel){
    set(epsRel,muRel);
    return;
}


void Material::set( double epsRel, double muRel){
    mEpsRecip = 1./(c_eps0*epsRel);
    mMuRecip  = 1./(c_mu0*muRel);
    return;
}

double Material::speed(){
    return c_c * sqrt(mEpsRecip*mMuRecip);
}

} // Namespace closure
#endif /* MATERIAL_HPP */
