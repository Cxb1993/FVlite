// point_source.hpp
//
// Simple source introduces a gaussian pulse point source in the center of the grid.
// Used for validation purposes.

#ifndef POINTSOURCE_HPP
#define POINTSOURCE_HPP

#include <iostream>
#include <cmath>

#include "Vectors/Grid.hpp"

namespace FVTD{

// Declare Gaussian function
double Gaussian( double, double, double);

class PointSource{

protected:

    Grid* pGrid;

public:

    PointSource( Grid* pGrid);
    virtual ~PointSource();

    void exec( double t);
};

PointSource::PointSource( Grid* pGrid) : pGrid(pGrid){}

PointSource::~PointSource(){}

void PointSource::exec( double t){
    int midX = (pGrid->Nx()+2*pGrid->bound()) / 2;
    int midY = (pGrid->Ny()+2*pGrid->bound()) / 2;
    double sigma = 2e-10;
    double mean  = sigma*4;
    double Hz = Gaussian( t, mean, sigma);
    double Ex = c_eta0 * Hz;
    StateVector State(Ex,0,0,0,0,Hz);
    pGrid->state(midX,midY) += State;
    return;
}

double Gaussian( double t, double mean, double sigma){
    double result = exp(-(t-mean)*(t-mean)/(2*sigma*sigma));
    std::cout << "Source: " << result << std::endl;
    return result;
}

}// Namespace closure
#endif /* POINTSOURCE_HPP */
