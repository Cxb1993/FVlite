// CutCellBoundaryStateGrid
//
// Stores conserved state vectors on boundary for finite volume cut-cell methods.

#ifndef CUTCELLBOUNDARYSTATEGRID_HPP
#define CUTCELLBOUNDARYSTATEGRID_HPP

#include "BaseGridCartesianCentred.hpp"
#include "Vectors/StateVector.hpp"

#include <vector>

namespace FVlite{

template<unsigned int dim>
class CutCellBoundaryStateGrid : public virtual BaseGridCartesianCentred<dim> {
    protected:
        std::vector<StateVector> mCutCellBoundaryStates;
    public:
        CutCellBoundaryStateGrid( double Ls[dim], unsigned int Ns[dim], unsigned int ghosts) 
            : BaseGridCartesianCentred<dim>(Ls,Ns,ghosts)
        {
            mCutCellBoundaryStates.resize(this->total_cells());
        }

        StateVector& boundary_state( unsigned int ii, unsigned int jj=0, unsigned int kk=0) {
            return mCutCellBoundaryStates[this->get_idx(ii,jj,kk)];
        }
};

}
#endif
