// BoundaryModuleAbstract.hpp
//
// Abstract boundary module.

#ifndef INITIALISATIONMODULEABSTRACT_HPP
#define INITIALISATIONMODULEABSTRACT_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <libconfig.h++>

#include "Grid/Grid.hpp"
#include "Utils/ObjectFactory.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{


class InitialisationModule{

public:

    InitialisationModule(){}
    virtual ~InitialisationModule(){}

    virtual void init( Setting& cfg) = 0;
    virtual double exec( double x, double y) = 0;

};

// Setting up factory

ObjectFactory<InitialisationModule> InitialisationModuleFactory;

}// Namespace closure
#endif /* INITIALISATIONMODULEABSTRACT_HPP */
