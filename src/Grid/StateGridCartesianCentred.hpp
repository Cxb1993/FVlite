// CartesianCentredStateGrid.hpp
//
// Stores conserved state vectors for finite volume methods.

#ifndef STATEGRIDCARTESIANCENTRED_HPP
#define STATEGRIDCARTESIANCENTRED_HPP

#include "BaseGridCartesianCentred.hpp"
#include "Vectors/StateVector.hpp"
#include "constants.hpp"

#include <vector>

namespace FVlite{

template<unsigned int dim>
class StateGridCartesianCentred : public virtual BaseGridCartesianCentred<dim> {
    protected:
        std::vector<StateVector> mStates;
    public:
        StateGridCartesianCentred() 
        {
            mStates.resize(this->total_cells());
        }

        StateVector& state( unsigned int ii, unsigned int jj=0, unsigned int kk=0) {
            return mStates[this->get_idx(ii,jj,kk)];
        }

        double max_speed();
};

// TODO 1D, 3D
template<>
double StateGridCartesianCentred<2>::max_speed(){
#ifdef MAXWELL
    double max = c_c; // Speed of light
#else
    double max = 0.;
    double local_sound_speed;
    double max_local_speed;
    double total_local_speed;
    for( int jj=this->start(DIM_Y); jj<this->end(DIM_Y); jj++){
        for( int ii=this->start(DIM_X); ii<this->end(DIM_X); ii++){
            max_local_speed = fabs(this->state(ii,jj).ux());
            max_local_speed = (fabs(this->state(ii,jj).uy()) > max_local_speed) ? fabs(this->state(ii,jj).uy()) : max_local_speed; 
            local_sound_speed = this->state(ii,jj).a();
            total_local_speed = max_local_speed + local_sound_speed;
            if( fabs(total_local_speed) > 3e8 ){
                std::cerr << "ERROR: SOUND SPEED BROKEN SPEED OF LIGHT! GRID CELL: (" << ii << "," <<jj<<")"<<std::endl;
                exit(EXIT_FAILURE);
            }
            max = (total_local_speed > max) ? total_local_speed : max;
        }
    }
#endif
    return max;
}

}
#endif
