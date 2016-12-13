// MaterialGridCartesianCentred.hpp
//
// Stores material properties for finite volume methods.
// Note that this may not actually be a grid for constant material problems!
// In cases where material is constant, it will only contain a single instance
// of Material and all indexed called will refer to that.

// For now, implemented for MaterialElectromagnetic only

#ifndef MATERIALGRIDCARTESIANCENTRED_HPP
#define MATERIALGRIDCARTESIANCENTRED_HPP

#include "BaseGridCartesianCentred.hpp"
#include "Vectors/MaterialElectromagnetic.hpp"
#include "constants.hpp"

#include <vector>

namespace FVlite{

template<unsigned int dim>
class MaterialGridCartesianCentred : public virtual BaseGridCartesianCentred<dim> {
    protected:
        std::vector<Material> mMats;
    public:
        MaterialGridCartesianCentred() 
        {
            mMats.resize(this->total_cells());
        }

        Material& material( unsigned int ii, unsigned int jj=0, unsigned int kk=0) {
            return mMats[this->get_idx(ii,jj,kk)];
        }
};

}
#endif
