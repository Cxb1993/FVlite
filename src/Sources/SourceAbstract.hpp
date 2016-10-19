// SourceAbstract.hpp
//
// Abstract source generator
//
// TODO: Generalise sources!

#ifndef SOURCEABSTRACT_HPP
#define SOURCEABSTRACT_HPP

#include "Vectors/StateVector.hpp"
#include "FluxSolvers/FluxSolvers.hpp"

namespace FVlite{

class Source{

public:

    Source();
    virtual ~Source();

    virtual StateVector exec( double t) = 0;
};

Source::Source(){}
Source::~Source(){}

}// Namespace closure
#endif /* SOURCEABSTRACT_HPP */
