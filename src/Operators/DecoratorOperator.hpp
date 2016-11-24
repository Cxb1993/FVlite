// DecoratorOperator.hpp
//
// Variation on Decorator which implements the exec(Grid& grid, Timer& timer) function

#ifndef DECORATOROPERATOR_HPP
#define DECORATOROPERATOR_HPP

#include "Operator.hpp"
#include "Utils/Decorator.hpp"
#include "Grid/Grid.hpp"
#include "Timer/Timer.hpp"

namespace FVlite{

template<class Wrapped>
class DecoratorOperator : public Decorator<Wrapped> {
protected:
    using Decorator<Wrapped>::mp_inner;
public:
    using Decorator<Wrapped>::Decorator;
    virtual void exec( Grid& grid, Timer& timer);
};

template<class Wrapped>
void DecoratorOperator<Wrapped>::exec( Grid& grid, Timer& timer){
    mp_inner->exec( grid, timer);
}

}
#endif
