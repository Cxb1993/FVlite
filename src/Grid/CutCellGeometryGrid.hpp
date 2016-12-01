// CutCellGeometryGrid.hpp
//
// Stores conserved state vectors for finite volume methods.

#ifndef CUTCELLGEOMETRYGRID_HPP
#define CUTCELLGEOMETRYGRID_HPP

#include "BaseGridCartesianCentred.hpp"
#include "BoundaryGeometry.hpp"

#include <vector>

namespace FVlite{

template<unsigned int dim>
class CutCellGeometryGrid : public virtual BaseGridCartesianCentred<dim> {
    protected:
        std::vector<BoundaryGeometry> mCutCellGeometries;
    public:
        CutCellGeometryGrid( double Ls[dim], unsigned int Ns[dim], unsigned int ghosts) 
            : BaseGridCartesianCentred<dim>(Ls,Ns,ghosts)
        {
            mCutCellGeometries.resize(this->total_cells());
        }

        BoundaryGeometry& boundary( unsigned int ii, unsigned int jj=0, unsigned int kk=0) {
            return mCutCellGeometries[this->get_idx(ii,jj,kk)];
        }
};

}
#endif
