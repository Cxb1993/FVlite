// MaterialGridCartesianCentred.hpp
//
// Stores material properties for finite volume methods.
// To be used in cases where material properties vary in space

#ifndef MATERIALGRIDCARTESIANCENTRED_HPP
#define MATERIALGRIDCARTESIANCENTRED_HPP

#include "BaseGridCartesianCentred.hpp"
#include "constants.hpp"

#include <vector>

namespace FVlite{

template<unsigned int dim, class Mat>
class MaterialGridCartesianCentred : public virtual BaseGridCartesianCentred<dim> {
    protected:
        std::vector<Mat> mMats;
    public:
        MaterialGridCartesianCentred() 
        {
            mMats.resize(this->total_cells());
        }

        Mat& material( unsigned int ii, unsigned int jj=0, unsigned int kk=0) {
            return mMats[this->get_idx(ii,jj,kk)];
        }
};

}
#endif
