// SignedDistanceAbstract.hpp
//
// Abstract class. Base of signed distance methods.

#ifndef SIGNEDDISTANCEABSTRACT_HPP
#define SIGNEDDISTANCEABSTRACT_HPP

#include <libconfig.h++>

#include "Utils/ObjectFactory.hpp"
#include "Vectors/Vector3.hpp"
#include "Utils/Geometry.hpp"

using libconfig::Setting;

namespace FVlite{

class SignedDistance {

public:

    SignedDistance(){}
    virtual ~SignedDistance(){}

    virtual void init( Setting& cfg) = 0;
    virtual double get_distance( const Vector3& pos) const = 0;

};

// Setting up factory

ObjectFactory<SignedDistance> SignedDistanceFactory;

}
#endif
