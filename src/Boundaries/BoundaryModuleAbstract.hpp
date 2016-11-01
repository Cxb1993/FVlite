// BoundaryModuleAbstract.hpp
//
// Abstract boundary module.

#ifndef BOUNDARYMODULEABSTRACT_HPP
#define BOUNDARYMODULEABSTRACT_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <libconfig.h++>

#include "Utils/ObjectFactory.hpp"
#include "BoundaryFunctions.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class BoundaryModule{
public:

    BoundaryModule(){}
    BoundaryModule( const BoundaryModule& other);
    virtual ~BoundaryModule(){}

    virtual void init( Setting& cfg);
    virtual StateVector getBoundary( const StateVector& Reference, const char dim, const double t) = 0;

};

// Setting up factory

ObjectFactory<BoundaryModule> BoundaryModuleFactory;

// Function definitions

void BoundaryModule::init( Setting& cfg){
    (void)cfg;
}

}// Namespace closure
#endif /* BOUNDARYMODULEABSTRACT_HPP */
