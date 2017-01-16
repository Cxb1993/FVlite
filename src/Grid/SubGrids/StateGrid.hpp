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

        unsigned int state_start( unsigned int dim){ return this->start(dim);}
        unsigned int state_end( unsigned int dim){ return this->end(dim);}
        double state_position( unsigned int dim, unsigned int ii){ return this->position(dim,ii);}
};

}
#endif
