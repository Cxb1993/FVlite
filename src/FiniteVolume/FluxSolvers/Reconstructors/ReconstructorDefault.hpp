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
    virtual StatePair exec( Grid& grid, double ds, double dt, char dim, int ii, int jj);
    virtual int stencilSize(){ return 2;}
};

REGISTER(Reconstructor,Default)

StatePair ReconstructorDefault::exec( Grid& grid, double ds, double dt, char dim, int ii, int jj){
    (void)ds;
    (void)dt;
    StateVector StateL = grid.state(ii,jj);
    StateVector StateR;
    switch(dim){
        case 'x' :
            StateR = grid.state(ii+1,jj);
            break;
        case 'y' :
            StateR = grid.state(ii,jj+1);
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
