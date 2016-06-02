// InitialiserStandard.hpp
//
// Finite volume grid initialisaer.
// Initialises entire grid to zeroes.
// Only applicable to MAXWELL

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
    for( int ii=pGrid->startX(); ii<pGrid->endX(); ii++){
        for( int jj=pGrid->startY(); jj<pGrid->endY(); jj++){
            pGrid->state(ii,jj)    = ZEROSTATE;
        }
    }
    return;
}

}// Namespace Closure
#endif /* INITIALISERZERO_HPP */
