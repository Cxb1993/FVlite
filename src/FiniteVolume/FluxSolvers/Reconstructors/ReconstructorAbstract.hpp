// ReconstructorAbstract.hpp
//
// Abstract reconstructor class for finite volume
// flux calculation.

#ifndef RECONSTRUCTORABSTRACT_HPP
#define RECONSTRUCTORABSTRACT_HPP

#include <cstdlib>
#include <cmath>
#include <utility>
#include <iostream>
#include <string>

#include <libconfig.h++>

#include "Grid/Grid.hpp"
#include "Limiters/Limiters.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

typedef std::pair<StateVector,StateVector> StatePair;

class Reconstructor {

protected:

    Grid* mpGrid;
    Limiter* mpLimiter;

public:

    Reconstructor(){}
    virtual ~Reconstructor();

    void init( Grid* pGrid, Setting& cfg);

    virtual StatePair exec( double ds, double dt, char dim, int ii, int jj)=0;
    virtual int stencilSize() = 0;
};

// Factory declaration

ObjectFactory<Reconstructor> ReconstructorFactory;

// Function definitions

Reconstructor::~Reconstructor(){
    delete mpLimiter;
}

void Reconstructor::init( Grid* pGrid, Setting& cfg){
    string limitType;
    // Get limiter. Rely on default if not specified
    try{
        limitType = cfg.lookup("limiter").c_str();
    } catch( const std::exception&) {
        limitType = "Default";
    }
    mpGrid = pGrid;
    mpLimiter = LimiterFactory.create(limitType);
    return;
}

} // namespace closure
#endif // RECONSTRUCTORABSTRACT_HPP
