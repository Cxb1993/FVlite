// InitialiserAbstract.hpp
//
// Abstract finite volume grid initialisaer.

#ifndef INITIALISERABSTRACT_HPP
#define INITIALISERABSTRACT_HPP

#include "Vectors/Grid.hpp"
#include "Timer/Timer.hpp"

namespace FVTD{

class Initialiser{

protected:

    Grid* pGrid;

public:

    Initialiser( Grid* pGrid);
    virtual ~Initialiser();

    virtual void exec() = 0;
};

Initialiser::Initialiser( Grid* pGrid) : pGrid(pGrid){
}

Initialiser::~Initialiser(){
}

}// Namespace closure
#endif /* INITIALISERABSTRACT_HPP */
