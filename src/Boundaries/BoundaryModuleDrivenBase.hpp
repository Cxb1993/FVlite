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
    virtual StateVector getBoundary( const StateVector& Reference, const char dim, const double t);
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

StateVector BoundaryModuleDrivenBase::getBoundary( const StateVector& Reference, const char dim, const double t){
    // TODO Make the boundary both transmissive and driven.
    //      As it stands, it is possible to generate instabilities when backscattered waves meet the boundary.
    (void)Reference;
    double driventerm = getDrivenTerm(t);
    StateVector Boundary;

    // Set to zero
    Boundary = ZEROSTATE;

    // Calculate driven part
    double electric, magnetic;
    electric = driventerm;
    magnetic = electric / c_eta0;

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
    
    return Boundary;
}


}// Namespace closure
#endif /* MAXWELL */
#endif /* BOUNDARYMODULEDRIVENBASE_HPP */
