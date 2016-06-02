// Initialisers.hpp
//
// Include file for finite volume grid initialisaers.

#ifndef INITIALISERS_HPP
#define INITIALISERS_HPP

#include "InitialiserAbstract.hpp"
#include "InitialiserZero.hpp"
#include "InitialiserCircle.hpp"
#include "InitialiserRectangle.hpp"
#include "InitialiserSlope.hpp"
#include "InitialiserExplosiveTest.hpp"

namespace FVTD{

enum INIT_TYPE{
   ZERO,
   CIRCLE,
   RECTANGLE,
   SLOPE,
   EXPLODE
};

}// Namespace closure
#endif /* INITIALISERS_HPP */
