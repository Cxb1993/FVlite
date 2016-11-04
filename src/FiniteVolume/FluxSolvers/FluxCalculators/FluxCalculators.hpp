// FluxCalculators.hpp
//
// include file for finite volume flux calculators

#ifndef FLUXCALCULATORS_HPP
#define FLUXCALCUALTORS_HPP

#include "FluxCalculatorLaxFriedrichs.hpp"
#include "FluxCalculatorRichtmyer.hpp"
#include "FluxCalculatorFORCE.hpp"

#ifdef EULER
#include "FluxCalculatorHLLC.hpp"
#endif

#endif // FLUXCALCULATORS_HPP
