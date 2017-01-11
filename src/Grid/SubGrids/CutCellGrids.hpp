// CutCellGrids.hpp

#ifndef CUTCELLGRIDS_HPP
#define CUTCELLGRIDS_HPP

#include "Grid/BoundaryGeometry.hpp"
#include "Vectors/StateVector.hpp"

#include <vector>

namespace FVlite{

template<class GridType>
class CutCellGeometryGrid : public virtual GridType {
    protected:
        std::vector<BoundaryGeometry> mCutCellGeometries;
    public:
        CutCellGeometryGrid() {
            mCutCellGeometries.resize(this->total_elements());
        }

        BoundaryGeometry& boundary( unsigned int ii, unsigned int jj=0, unsigned int kk=0) {
            return mCutCellGeometries[this->get_idx(ii,jj,kk)];
        }
};

template<class GridType>
class CutCellBoundaryStateGrid : public virtual GridType {
    protected:
        std::vector<StateVector> mCutCellBoundaryStates;
    public:
        CutCellBoundaryStateGrid() {
            mCutCellBoundaryStates.resize(this->total_elements());
        }

        StateVector& boundary_state( unsigned int ii, unsigned int jj=0, unsigned int kk=0) {
            return mCutCellBoundaryStates[this->get_idx(ii,jj,kk)];
        }
};

}
#endif
