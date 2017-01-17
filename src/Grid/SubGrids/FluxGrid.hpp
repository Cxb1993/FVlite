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

        unsigned int flux_start( unsigned int dim) const { return this->start(dim);}
        unsigned int flux_end( unsigned int dim) const { return this->end(dim);}
        unsigned int num_fluxes( unsigned int dim) const { return this->num_elements(dim);}
        unsigned int full_fluxes( unsigned int dim) const { return this->full_elements(dim);}
        unsigned int total_fluxes() const { return this->total_elements();}
        double flux_position( unsigned int dim, unsigned int ii) const { return this->position(dim,ii);}
};

}
#endif
