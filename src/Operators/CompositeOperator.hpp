// CompositeOperator.hpp
//
// Variation on Composite<Operator> which can call the
// function exec( Grid& grid, Timer& timer).
//
// This code will likely be replaced with a more 'elegant' solution
// in the near future. This will not be needed if we remove the constraint that
// all Operators require a common interface.

#ifndef COMPOSITEOPERATOR_HPP
#define COMPOSITEOPERATOR_HPP

#include "Operator.hpp"
#include "Utils/Composite.hpp"
#include "Grid/Grid.hpp"
#include "Timer/Timer.hpp"

namespace FVlite{

class CompositeOperator : public Composite<Operator> {
protected:
    using Composite<Operator>::m_elements;
public:
    void exec( Grid& grid, Timer& timer);
};

void CompositeOperator::exec( Grid& grid, Timer& timer){
    for( auto iter = m_elements.begin(); iter != m_elements.end(); iter++){
        (*iter)->exec(grid,timer);
    }
}

}
#endif
