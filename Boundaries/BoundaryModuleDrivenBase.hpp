// BoundaryModuleDrivenBase.hpp
//
// Abstract boundary module.
// Base class for driven boundaries

#ifndef BOUNDARYMODULEDRIVENBASE_HPP
#define BOUNDARYMODULEDRIVENBASE_HPP

#ifdef MAXWELL

#include <string>
#include <libconfig.h++>

#include "Boundaries/BoundaryModuleAbstract.hpp"
#include "constants.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

enum POLARISATION{
    TE,
    TM
};

class BoundaryModuleDrivenBase : public BoundaryModule {
protected:
    POLARISATION mPOL;
public:
    virtual void init( Grid* pGrid, Setting& cfg);
    virtual StateVector getBoundary( const StateVector& Reference, const char dim, const double t, const double dt);
    virtual double getDrivenTerm( const double t) = 0;
};


void BoundaryModuleDrivenBase::init( Grid* pGrid, Setting& cfg){
    BoundaryModule::init(pGrid,cfg);
    Setting& params = cfg.lookup("params");
    string polString = params.lookup("polarisation");
    if( polString == "x" || polString == "TE") mPOL = TE;
    if( polString == "y" || polString == "TM") mPOL = TM;
    return;
}

StateVector BoundaryModuleDrivenBase::getBoundary( const StateVector& Reference, const char dim, const double t, const double dt){
    double driventerm = getDrivenTerm(t);
    double lastdriventerm = getDrivenTerm(t-dt); // Source of error: dt might change!
    StateVector Boundary;

    // Use transmissive boundary condition
    Boundary = Boundary::Transmissive(Reference);

    // Calculate driven part
    double electric, magnetic;
    electric = (t==0.) ? driventerm : driventerm - lastdriventerm;
    magnetic = electric/c_eta0;

    // Add to boundary
    switch(dim){
        case 'x':
            switch(mPOL){
                case TE: 
                    Boundary.Ey() += electric;
                    Boundary.Hz() += magnetic;
                    break;
                case TM:
                    Boundary.Ez() += electric;
                    Boundary.Hy() += magnetic;
                    break;
            }
            break;
        case 'y':
            switch(mPOL){
                case TE: 
                    Boundary.Ex() += electric;
                    Boundary.Hz() += magnetic;
                    break;
                case TM:
                    Boundary.Ez() += electric;
                    Boundary.Hx() += magnetic;
                    break;
            }
            break;
    }
    
 /*   std::cout<<"electric: " << electric<<std::endl;    
    std::cout<<"magnetic: " << magnetic<<std::endl;    
    std::cout<<"t: " << t<<std::endl;
*/
    return Boundary;
}


}// Namespace closure
#endif /* MAXWELL */
#endif /* BOUNDARYMODULEDRIVENBASE_HPP */
