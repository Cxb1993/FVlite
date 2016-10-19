// SourceNone.hpp
//
// Does not generate source. Used whenever MAXWELL not defined.

#ifndef SOURCENONE_HPP
#define SOURCENONE_HPP

#include <iostream>
#include <cmath>

#include "SourceAbstract.hpp"
#include "Vectors/StateVector.hpp"

namespace FVlite{

// Begin class
class SourceNone : public Source{

public:

    SourceNone();
    virtual ~SourceNone();
    virtual StateVector exec( double t);
};

SourceNone::SourceNone(){}

SourceNone::~SourceNone(){}

StateVector SourceNone::exec( double t){
    (void)t;
    return ZEROSTATE;
}

}// Namespace closure
#endif /* SOURCENONE_HPP */
