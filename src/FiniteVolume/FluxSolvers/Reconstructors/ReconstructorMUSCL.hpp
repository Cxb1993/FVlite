// ReconstructorMUSCL.hpp
//
// Finite volume state reconstructor for the MUSCL method.

#ifndef RECONSTRUCTORMUSCL_HPP
#define RECONSTRUCTORMUSCL_HPP

#include"ReconstructorAbstract.hpp"

#ifdef EULER
#define DEFAULTTOL 1e-10
#endif

#ifdef MAXWELL
#define DEFAULTTOL 1e-20
#endif

namespace FVlite{

class ReconstructorMUSCL : public Reconstructor{
public:
    virtual StatePair exec( Grid& grid, double ds, double dt, char dim, int ii, int jj);
    virtual int stencilSize(){ return 4;}
    
    StateVector getLimitedSlope(
        StateVector& StateL,
        StateVector& StateC,
        StateVector& StateR,
        double tol=DEFAULTTOL,
        double omega=0.
    );
};

REGISTER(Reconstructor,MUSCL)

StatePair ReconstructorMUSCL::exec( Grid& grid, double ds, double dt, char dim, int ii, int jj){
    BoundaryGeometry BoundaryL, BoundaryCL, BoundaryCR, BoundaryR;
    StateVector StateL, StateCL, StateCR, StateR;
    Material MatCL, MatCR;
    StatePair States;

    // Get States
    switch(dim){
        case 'x':
            BoundaryL  = grid.boundary(ii-1,jj);
            BoundaryCL = grid.boundary(ii,jj);
            BoundaryCR = grid.boundary(ii+1,jj);
            BoundaryR  = grid.boundary(ii+2,jj);
            MatCL = grid.material(ii,jj);
            MatCR = grid.material(ii+1,jj);
            StateL  = grid.state(ii-1,jj);
            StateCL = grid.state(ii,jj);
            StateCR = grid.state(ii+1,jj);
            StateR  = grid.state(ii+2,jj);
            break;
        case 'y':
            BoundaryL  = grid.boundary(ii,jj-1);
            BoundaryCL = grid.boundary(ii,jj);
            BoundaryCR = grid.boundary(ii,jj+1);
            BoundaryR  = grid.boundary(ii,jj+2);
            MatCL = grid.material(ii,jj);
            MatCR = grid.material(ii,jj+1);
            StateL  = grid.state(ii,jj-1);
            StateCL = grid.state(ii,jj);
            StateCR = grid.state(ii,jj+1);
            StateR  = grid.state(ii,jj+2);
            break;
        case 'z':
            // Not yet implemented
            break;
    }

    // Handle case in which stencil includes some mixed cells
    if( BoundaryL.alpha()<1.
        || BoundaryCL.alpha()<1.
        || BoundaryCR.alpha()<1. 
        || BoundaryR.alpha()<1.)
    {
        States = std::make_pair(StateCL,StateCR);
        return States;
    }
    
    // Perform linear reconstruction
    StateVector DeltaL, DeltaR;
    StateVector InterL, InterR;
    FluxVector FluxL, FluxR;
    StateVector StateRbar, StateLbar;

    DeltaL = getLimitedSlope( StateL, StateCL, StateCR);
    DeltaR = getLimitedSlope( StateCL, StateCR, StateR);

    // Reconstruction on left
    //
    // Step 1: Data reconstruction
    InterL = StateCL - 0.5*DeltaL;
    InterR = StateCL + 0.5*DeltaL;
    //
    // Step 2: Get fluxes
    FluxL.set(InterL,MatCL,dim);
    FluxR.set(InterR,MatCR,dim);
    //
    // Set 3: Evolve half timestep, get boundary state
    StateRbar = InterR + 0.5*(dt/ds)*( FluxL - FluxR);

    // Reconstruction on right
    //
    // Step 1: Data reconstruction
    InterL = StateCR - 0.5*DeltaR;
    InterR = StateCR + 0.5*DeltaR;
    //
    // Step 2: Get Fluxes
    FluxL.set(InterL,MatCR,dim);
    FluxR.set(InterR,MatCR,dim);
    // Set 3: Evolve half timestep, get boundary state
    StateLbar = InterL + 0.5*(dt/ds)*( FluxL - FluxR);

    States = std::make_pair(StateRbar,StateLbar);
    return States;
}

StateVector ReconstructorMUSCL::getLimitedSlope(StateVector& StateL, StateVector& StateC, StateVector& StateR, double tol, double omega){
    StateVector  DeltaL, DeltaR, r, Limiter, Delta;
    DeltaL = StateC - StateL;
    DeltaR = StateR - StateC;;
    // Impose minimum jump conditions
    for( unsigned int kk=0; kk<DeltaL.size(); kk++){
        if(fabs(DeltaL[kk]) < tol) DeltaL[kk] = tol*copysign(1.0,DeltaL[kk]);
        if(fabs(DeltaR[kk]) < tol) DeltaR[kk] = tol*copysign(1.0,DeltaR[kk]);
    }
    r = DeltaL/DeltaR; // component wise division
    Limiter = mpLimiter->exec( r, omega);
    Delta = 0.5*Limiter*((1+omega)*DeltaL + (1-omega)*DeltaR); // Weight to one direction or the other. By default, no weighting, omega=0.
    return Delta;
}

}// Namespace closure
#endif /* RECONSTRUCTORMUSCL_HPP */
