// StateVector.hpp
// Allows conditional compiling for different sets of equations.
// See those files for implementations

// Maxwell's equations
#ifdef MAXWELL
#include "StateVectorMaxwell.hpp"
#endif

// Euler equations
#ifdef EULER
#include "StateVectorEuler.hpp"
#endif
