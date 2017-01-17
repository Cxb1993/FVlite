// CentredStateGrid.hpp
//
// Stores conserved state vectors for finite volume methods.

#ifndef STATEGRID_HPP
#define STATEGRID_HPP

#include "Vectors/StateVector.hpp"

#include <vector>

namespace FVlite{

template<class GridType>
class StateGrid : public virtual GridType {
    protected:
        std::vector<StateVector> mStates;
    public:
        StateGrid() 
        {
            mStates.resize(this->total_elements());
        }

        StateVector& state( unsigned int ii, unsigned int jj=0, unsigned int kk=0) {
            return mStates[this->get_idx(ii,jj,kk)];
        }

        unsigned int state_start( unsigned int dim) const { return this->start(dim);}
        unsigned int state_end( unsigned int dim) const { return this->end(dim);}
        unsigned int num_states( unsigned int dim) const { return this->num_elements(dim);}
        unsigned int full_states( unsigned int dim) const { return this->full_elements(dim);}
        unsigned int total_states() const { return this->total_elements();}
        double state_position( unsigned int dim, unsigned int ii) const { return this->position(dim,ii);}
};

}
#endif
