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

template<class DerivedOperator>
class CompositeOperator : public Composite<DerivedOperator> {
protected:
    using Composite<DerivedOperator>::m_elements;
public:
    void exec( Grid& grid, Timer& timer);
};

template<class DerivedOperator>
void CompositeOperator<DerivedOperator>::exec( Grid& grid, Timer& timer){
    typename std::vector<DerivedOperator*>::iterator iter;
    for( iter = m_elements.begin(); iter != m_elements.end(); iter++){
        (*iter)->exec(grid,timer);
    }
}

}
#endif
