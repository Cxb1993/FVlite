// FluxGrid
//
// Stores flux vectors for finite volume methods.

#ifndef FLUXGRID_HPP
#define FLUXGRID_HPP

#include "Vectors/FluxVector.hpp"

#include <vector>

namespace FVlite{

template<class GridType>
class FluxGrid : public virtual GridType {
    protected:
        std::vector<FluxVector> mFluxes;
    public:
        FluxGrid() 
        {
            mFluxes.resize(this->total_elements());
        }

        FluxVector& flux( unsigned int ii, unsigned int jj=0, unsigned int kk=0) {
            return mFluxes[this->get_idx(ii,jj,kk)];
        }
};

}
#endif
