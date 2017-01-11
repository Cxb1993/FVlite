// ConstantGrid
// 
// Contains base class for constant grids.
// These are used to specify constant parameters across the grid
// without requiring their storage across the grid.

#ifndef CONSTANTGRID_HPP
#define CONSTANTGRID_HPP

#include "BaseGrid.hpp"

namespace FVlite{

class ConstantGrid : public BaseGrid {
public:
    ConstantGrid(){}
    virtual ~ConstantGrid(){}

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
