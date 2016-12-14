// CartesianCentredStateGrid.hpp
//
// Stores conserved state vectors for finite volume methods.

#ifndef STATEGRIDCARTESIANCENTRED_HPP
#define STATEGRIDCARTESIANCENTRED_HPP

#include "BaseGridCartesianCentred.hpp"
#include "Vectors/StateVector.hpp"
#include "constants.hpp"

#include <vector>

namespace FVlite{

template<unsigned int dim>
class StateGridCartesianCentred : public virtual BaseGridCartesianCentred<dim> {
    protected:
        std::vector<StateVector> mStates;
    public:
        StateGridCartesianCentred() 
        {
            mStates.resize(this->total_cells());
        }

        StateVector& state( unsigned int ii, unsigned int jj=0, unsigned int kk=0) {
            return mStates[this->get_idx(ii,jj,kk)];
        }

};

}
#endif
