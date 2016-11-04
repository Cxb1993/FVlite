// FluxCalculatorAbstract.hpp
//
// Finite volume flux calculator
// A functor which takes two states and returns the intercell flux

#ifndef FLUXABSTRACT_HPP
#define FLUXABSTRACT_HPP

#include "Grid/Grid.hpp" // for StateVectors etc
#include "BasicFluxFunctions.hpp"

namespace FVlite{

class FluxCalculator {
    public:
    FluxCalculator(){}
    virtual ~FluxCalculator(){}
    virtual FluxVector exec(
        double ds, double dt, char dim, 
        const StateVector& L, const StateVector& R
    ) = 0;
};

ObjectFactory<FluxCalculator> FluxCalculatorFactory;

} // Namespace closure 
#endif // FLUXABSTRACT_HPP
