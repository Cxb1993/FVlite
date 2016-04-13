// BoundaryUpdaters.hpp
//
// Include file for finite volume grid boundary updaters.

#ifndef BUPDATERS_HPP
#define BUPDATERS_HPP

#include "BoundaryUpdaterAbstract.hpp"
#include "BoundaryUpdaterTransmissive.hpp"
#include "BoundaryUpdaterConstant.hpp"

namespace FVTD{

enum B_UPDATE_TYPE{
    TRANSMISSIVE,
    CONSTANT
};

}// Namespace closure
#endif /* BUPDATERS_HPP */
