// CutCellGrids.hpp

#ifndef CUTCELLGRIDS_HPP
#define CUTCELLGRIDS_HPP

#include "BaseGridCartesianCentred.hpp"
#include "BoundaryGeometry.hpp"
#include "Vectors/StateVector.hpp"

#include <vector>

namespace FVlite{

template<unsigned int dim>
class CutCellGeometryGrid : public virtual BaseGridCartesianCentred<dim> {
    protected:
        std::vector<BoundaryGeometry> mCutCellGeometries;
    public:
        CutCellGeometryGrid() {
            mCutCellGeometries.resize(this->total_cells());
        }

        BoundaryGeometry& boundary( unsigned int ii, unsigned int jj=0, unsigned int kk=0) {
            return mCutCellGeometries[this->get_idx(ii,jj,kk)];
        }
};

template<unsigned int dim>
class CutCellBoundaryStateGrid : public virtual BaseGridCartesianCentred<dim> {
    protected:
        std::vector<StateVector> mCutCellBoundaryStates;
    public:
        CutCellBoundaryStateGrid() {
            mCutCellBoundaryStates.resize(this->total_cells());
        }

        StateVector& boundary_state( unsigned int ii, unsigned int jj=0, unsigned int kk=0) {
            return mCutCellBoundaryStates[this->get_idx(ii,jj,kk)];
        }
};

}
#endif
