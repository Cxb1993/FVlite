// MaterialGridConstant.hpp
//
// Stores material properties for finite volume methods.
// To be used in cases where material properties are universally constant.
// Though it has the appearance of the grid, this is merely to allow a 
// common interface with MaterialGridCartesianCentred.

#ifndef MATERIALGRIDCONSTANT_HPP
#define MATERIALGRIDCONSTANT_HPP

namespace FVlite{

// TODO Should inherit BaseGrid. Not CartesianCentred. Virtual inheritance issues.

template< class Mat>
class MaterialGridConstant : public virtual BaseGridCartesianCentred<2> {
    protected:
        Mat mMat;
    public:
        MaterialGridConstant() {}

        Mat& material( unsigned int ii, unsigned int jj=0, unsigned int kk=0) {
            (void)ii;
            (void)jj;
            (void)kk;
            return mMat;
        }
};

}
#endif
