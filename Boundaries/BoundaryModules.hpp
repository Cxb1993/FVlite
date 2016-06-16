// BoundaryModules.hpp
//
// Include file for boundary modules.
// Each module takes care of a specified area of the grid and applies a
// boundary condition over that area.

#ifndef BOUNDARYMODULES_HPP
#define BOUNDARYMODULES_HPP

#include "BoundaryModuleAbstract.hpp"
#include "BoundaryModuleTransmissive.hpp"
#include "BoundaryModuleReflective.hpp"

#ifdef MAXWELL
#include "BoundaryModuleSine.hpp"
#include "BoundaryModuleCosine.hpp"
#include "BoundaryModuleGaussian.hpp"
#include "BoundaryModuleGaussDer.hpp"
#endif

#endif /* BOUNDARYMODULES_HPP */
