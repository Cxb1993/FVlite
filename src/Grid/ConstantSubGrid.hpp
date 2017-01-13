// ConstantSubGrid
// 
// Contains base class for constant grids.
// These are used to specify constant parameters across the grid
// without requiring their storage at each point in space

#ifndef CONSTANTGRID_HPP
#define CONSTANTGRID_HPP

#include "BaseGrid.hpp"

namespace FVlite{

class ConstantSubGrid : public BaseGrid {
public:
    ConstantSubGrid(){}
    virtual ~ConstantSubGrid(){}

    unsigned int total_elements(){
        return 1;
    }

    unsigned int get_idx( unsigned int ii, unsigned int jj=0, unsigned int kk=0) const{
        (void)ii;
        (void)jj;
        (void)kk;
        return 0;
    }   
};

}
#endif
