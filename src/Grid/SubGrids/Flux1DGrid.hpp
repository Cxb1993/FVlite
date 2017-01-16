// Flux1DGrid
//
// Stores flux vectors for finite volume methods.
//
// Variation in which the flux grid stores an
// extra element in each direction so that it may
// be used as an face-centred grid during any
// directional sweep.
//
// In practice, this means the end(dim) instead takes
// two arguments, becoming end(dim,sweep_dim). If
// dim==sweep_dim, it works as usual for a vertex
// centred grid (i.e. one more element than cells).
// Otherwise, the end point is reduced by one.
//
// Functions such as num_fluxes, total_fluxes etc
// not defined.

#ifndef FLUX1DGRID_HPP
#define FLUX1DGRID_HPP

#include "Vectors/FluxVector.hpp"

#include <vector>

namespace FVlite{

template<unsigned int global_dim>
class Flux1DGrid : public virtual CartesianSubGrid<global_dim,VertexCentred,NoGhosts> {
    protected:
        std::vector<FluxVector> mFluxes;
    public:
        Flux1DGrid() 
        {
            mFluxes.resize(this->total_elements());
        }

        FluxVector& flux( unsigned int ii, unsigned int jj=0, unsigned int kk=0) {
            return mFluxes[this->get_idx(ii,jj,kk)];
        }

        unsigned int flux_start( unsigned int dim) const { return this->start(dim);}
        double flux_position( unsigned int dim, unsigned int ii) const { return this->position(dim,ii);}

        unsigned int flux_end( unsigned int dim, unsigned int sweep_dim){
            return this->end(dim) - (dim != sweep_dim);
        }

};

}
#endif
