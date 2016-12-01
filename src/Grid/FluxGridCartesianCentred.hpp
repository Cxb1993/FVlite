// FluxGridCartesianCentred
//
// Stores flux vectors for finite volume methods.

#ifndef FLUXGRIDCARTESIANCENTRED_HPP
#define FLUXGRIDCARTESIANCENTRED_HPP

#include "BaseGridCartesianCentred.hpp"
#include "Vectors/FluxVector.hpp"

#include <vector>

namespace FVlite{

template<unsigned int dim>
class FluxGridCartesianCentred : public virtual BaseGridCartesianCentred<dim> {
    protected:
        std::vector<FluxVector> mFluxes;
    public:
        FluxGridCartesianCentred() 
        {
            mFluxes.resize(this->total_cells());
        }

        FluxVector& flux( unsigned int ii, unsigned int jj=0, unsigned int kk=0) {
            return mFluxes[this->get_idx(ii,jj,kk)];
        }
};

}
#endif
