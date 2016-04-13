// InitialiserStandard.hpp
//
// Finite volume grid initialisaer.
// Initialises entire grid to zeroes.

#ifndef INITIALISERZERO_HPP
#define INITIALISERZERO_HPP

#include"InitialiserAbstract.hpp"

#include <cmath>

namespace FVTD{

class InitialiserZero : public Initialiser{
public:
    InitialiserZero( Grid* pGrid) : Initialiser(pGrid) {}
    virtual void exec();
};

void InitialiserZero::exec(){
    StateVector State(0,0,0,0,0,0);
    Material    Mat(1.0,1.0);
    for( int ii=pGrid->startX(); ii<pGrid->endX(); ii++){
        for( int jj=pGrid->startY(); jj<pGrid->endY(); jj++){
            pGrid->state(ii,jj)    = State;
            pGrid->material(ii,jj) = Mat;
        }
    }
    return;
}

}// Namespace Closure
#endif /* INITIALISERZERO_HPP */
