// FluxVector.hpp
// Allows conditional compiling of different sets of equations.
// See these files for implementations

#ifdef MAXWELL
#include "FluxVectorMaxwell.hpp"
#endif

#ifdef EULER
#include "FluxVectorEuler.hpp"
#endif
