// Limiters.hpp
//
// Include file for finite volume grid updaters limiters.

#ifndef LIMITERS_HPP
#define LIMITERS_HPP

#include"LimiterAbstract.hpp"
#include"LimiterSuperBee.hpp"
#include"LimiterVanLeer.hpp"
#include"LimiterMinBee.hpp"
#include"LimiterNone.hpp"

namespace FVTD{

enum LIMIT_TYPE{
    NONE,
    SUPERBEE,
    VANLEER,
    MINBEE
};

}// Namespace closure
#endif /* LIMITERS_HPP */
