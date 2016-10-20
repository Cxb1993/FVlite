// ReconstructorDefault.hpp
//
// Finite volume state reconstructor.
// Implements trivial case in which no reconstruction occurs

#ifndef RECONSTRUCTORDEFAULT_HPP
#define RECONSTRUCTORDEFAULT_HPP
 
#include "ReconstructorAbstract.hpp"

namespace FVlite{

class ReconstructorDefault : public Reconstructor{
public:
    virtual StatePair exec( double ds, double dt, char dim, int ii, int jj);
    virtual int stencilSize(){ return 2;}
};

REGISTER(Reconstructor,Default)

StatePair ReconstructorDefault::exec( double ds, double dt, char dim, int ii, int jj){
    (void)ds;
    (void)dt;
    StateVector StateL = mpGrid->state(ii,jj);
    StateVector StateR;
    switch(dim){
        case 'x' :
            StateR = mpGrid->state(ii+1,jj);
            break;
        case 'y' :
            StateR = mpGrid->state(ii,jj+1);
            break;
        case 'z' :
            // Not yet implemented
            break;
    }
    StatePair States = std::make_pair(StateL,StateR);
    return States;
}

}// Namespace closure
#endif /* RECONSTRUCTORDEFAULT_HPP */
