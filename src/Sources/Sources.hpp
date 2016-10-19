// Sources.hpp
//
// Include file for source generators.

#ifndef SOURCES_HPP
#define SOURCES_HPP

#include "SourceAbstract.hpp"
#include "SourceNone.hpp"

//Maxwell sources
#ifdef MAXWELL
#include "SourceGaussian.hpp"
#include "SourceGaussder.hpp"
#include "SourceSin.hpp"
#include "SourceCos.hpp"
#endif

namespace FVlite{

enum SOURCE_TYPE{
    NOSOURCE,
    GAUSSIAN,
    GAUSSDER,
    SINE,
    COSINE
};


}// Namespace closure
#endif /* SOURCES_HPP */
