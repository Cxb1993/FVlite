// PDEsolvers.hpp
//
// Include file for finite volume PDE solvers.

#ifndef PDESOLVERS_HPP
#define PDESOLVERS_HPP

#include"PDEsolverAbstract.hpp"
#include"PDEsolverFORCE.hpp"
#include"PDEsolverRichtmyer.hpp"
#include"PDEsolverSLIC.hpp"
#include"Limiters/Limiters.hpp"

namespace FVTD{

enum PDE_TYPE{
    FORCE,
    RICHT,
    SLIC
};

}// Namespace closure
#endif /* PDESOLVERS_HPP */
