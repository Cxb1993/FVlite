// MaterialGridCartesianCentred.hpp
//
// Stores material properties for finite volume methods.
// To be used in cases where material properties vary in space

#ifndef MATERIALGRID_HPP
#define MATERIALGRID_HPP

#include <vector>

namespace FVlite{

template<class Mat, class GridType>
class MaterialGrid : public virtual GridType {
    protected:
        std::vector<Mat> mMats;
    public:
        MaterialGrid() 
        {
            mMats.resize(this->total_elements());
        }

        Mat& material( unsigned int ii, unsigned int jj=0, unsigned int kk=0) {
            return mMats[this->get_idx(ii,jj,kk)];
        }
};

}
#endif
