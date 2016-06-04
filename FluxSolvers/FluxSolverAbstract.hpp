// FluxSolverAbstract.hpp
//
// Abstract finite volume flux solver.

#ifndef FLUXABSTRACT_HPP
#define FLUXABSTRACT_HPP

#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <ctgmath>
#include <libconfig.h++>

#include "Grid/Grid.hpp"
#include "Timer/Timer.hpp"
#include "Sources/Sources.hpp"
#include "Limiters/Limiters.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{


class FluxSolver{

protected:

    Grid*    pGrid;
    Source*  pSource;
    Limiter* pLimiter;

public:

    FluxSolver(){}
    virtual ~FluxSolver();

    void init( Grid* pGrid, Source* pSource, string LimitString);
    void init( Grid* pGrid, Source* pSource, Setting& cfg);
    void exec( char dim, double t, double dt);
    virtual void solve(char dim, double dt) = 0;
    void inject_source( char dim, double t, double dt);

    // Declaring flux calculation functions.
    // Static functions, to be called by derived solvers.
    // These are included here as they are commonly used to build more complicated solvers.
    static FluxVector LaxFriedrichs_flux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR);
    static FluxVector Richtmyer_flux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR);
    static FluxVector FORCE_flux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR);
#ifdef EULER
    static FluxVector HLLC_flux( char dim, StateVector& StateL, StateVector& StateR); 
#endif

};

// Factory declaration

ObjectFactory<FluxSolver> FluxSolverFactory;


// Function defintions

void FluxSolver::init( Grid* pGrid, Source* pSource, string LimitString){
    (*this).pGrid = pGrid;
    (*this).pSource = pSource;
    pLimiter = LimiterFactory.create(LimitString);
}

void FluxSolver::init( Grid* pGrid, Source* pSource, Setting& cfg){
    string limitType = cfg.lookup("limiter");
    pLimiter = LimiterFactory.create(limitType);
    (*this).pGrid = pGrid;
    (*this).pSource = pSource;
    return;
}


FluxSolver::~FluxSolver(){
    delete pLimiter;
}

void FluxSolver::exec( char dim, double t, double dt){
    solve(dim,dt);
    inject_source(dim,t,dt);
    return;
}

void FluxSolver::inject_source( char dim, double t, double dt){
#ifdef MAXWELL
    if( dim != 'x' ) return;
    StateVector Source, StateR;
    Source = pSource->exec(t);
    double ds = pGrid->dx();
    int startX = pGrid->startX();
    int endX   = pGrid->endX();
    int startY = pGrid->startY();
    int endY   = pGrid->endY();
    FluxVector Flux;
    (void)endX;
    for( int jj=startY; jj<endY; jj++){
        StateR = pGrid->state(startX,jj);
        Flux = Richtmyer_flux(ds,dt,dim,Source,StateR);
        pGrid->flux(startX-1,jj) = Flux;
    }
#else
    (void)dim;
    (void)t;
    (void)dt;
#endif
    return;
}

FluxVector FluxSolver::LaxFriedrichs_flux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR){
    FluxVector FL, FR, result;
    FL.set(UL,dim);
    FR.set(UR,dim);
    result = 0.5*(FL+FR+(ds/dt)*(UL-UR));
    return result;
}

FluxVector FluxSolver::Richtmyer_flux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR){
    FluxVector FL, FR, result;
    StateVector Inter;
    FL.set(UL,dim);
    FR.set(UR,dim);
    Inter = 0.5*(UL+UR+(dt/ds)*(FL-FR));
    result.set(Inter,dim);
    return result;
}

FluxVector FluxSolver::FORCE_flux( double ds, double dt, char dim, const StateVector& UL, const StateVector& UR){
    FluxVector result;
    result = 0.5*(FluxSolver::LaxFriedrichs_flux(ds,dt,dim,UL,UR)
                  +FluxSolver::Richtmyer_flux(ds,dt,dim,UL,UR));
    return result; 
}

#ifdef EULER

FluxVector FluxSolver::HLLC_flux( char dim, StateVector& StateL, StateVector& StateR){
    // See 'Riemann Solvers & Numerical Methods for Fluid Dynamics',E Toro, section 10.6, for a summary of the HLLC fluxes

    // Setup
    const double gam = c_gamma_ideal;
    double rho_L = StateL.rho();
    double rho_R = StateR.rho();
    double p_L   = StateL.p();
    double p_R   = StateR.p();
    double a_L   = StateL.a();
    double a_R   = StateR.a();
    double u_L, u_R;
    int speed_idx;
    switch(dim){
        case 'x' :
            speed_idx = 1;
            u_L = StateL.ux();
            u_R = StateR.ux();
            break;
        case 'y' :
            speed_idx = 2;
            u_L = StateL.uy();
            u_R = StateR.uy();
            break;
        default  :
            std::cerr << "ERROR, INCORRECT DIM, HLLC FLUX SOLVER" << std::endl;
            exit(EXIT_FAILURE);
    } 

    // Step I: Pressure estimate.
    // Compute estimate for the pressure, p_star, in the 'star region'.
    double rho_bar = 0.5*(rho_L+rho_R);
    double a_bar   = 0.5*(a_L+a_R);
    double p_pvrs  = 0.5*(p_L+p_R) - 0.5*(u_R-u_L)*rho_bar*a_bar;
    double p_star  = fmax( 0., p_pvrs);

    // Step II: Wave speed estimates.
    // Compute the wave speed estimates, Sl and Sr, for the left and right states.
    double q_L, q_R;
    if( p_star <= p_L){
        q_L = 1.;
    } else {
        q_L = sqrt(1+(gam+1)*(p_star/p_L-1)/(2*gam));
    }
    if( p_star <= p_R){
        q_R = 1.;
    } else {
        q_R = sqrt(1+(gam+1)*(p_star/p_R-1)/(2*gam));
    }

    if( !std::isfinite(q_L)){std::cerr<<"q_L not finite"<<std::endl; exit(EXIT_FAILURE);}
    if( !std::isfinite(q_R)){std::cerr<<"q_R not finite"<<std::endl; exit(EXIT_FAILURE);}

    double S_L = u_L - a_L*q_L;
    double S_R = u_R + a_R*q_R;
    // Calculation of S_star is delayed

    // Step III: Calculate HLLC flux
    // Determine which regime we're in, and calculate accordingly.
    FluxVector Flux_HLLC;

    if( S_L >= 0.){
        Flux_HLLC.set(StateL,dim);
        return Flux_HLLC;
    }

    if( S_R <= 0.){
        Flux_HLLC.set(StateR,dim);
        return Flux_HLLC;
    }

    double delta_L = S_L - u_L;
    double delta_R = S_R - u_R;
    double S_star = (p_R - p_L + rho_L*u_L*delta_L - rho_R*u_R*delta_R) / (rho_L*delta_L - rho_R*delta_R);
    StateVector State_star;
    FluxVector Flux;

    if( S_star >= 0){ // F_star_L
        State_star = StateL/rho_L;
        State_star[speed_idx] = S_star;
        State_star[StateVector::size()-1] = StateL.E()/rho_L + (S_star-u_L)*(S_star + p_L/(rho_L*delta_L));
        State_star *= rho_L*delta_L/(S_L-S_star);
        Flux.set(StateL,dim);
        Flux_HLLC = Flux + S_L*(State_star - StateL);
    } else { // F_star_R
        State_star = StateR/rho_R;
        State_star[speed_idx] = S_star;
        State_star[StateVector::size()-1] = StateR.E()/rho_R + (S_star-u_R)*(S_star + p_R/(rho_R*delta_R));
        State_star *= rho_R*delta_R/(S_R-S_star);
        Flux.set(StateR,dim);
        Flux_HLLC = Flux + S_R*(State_star - StateR);
    }
    return Flux_HLLC;
}
#endif /* EULER */

}// Namespace closure
#endif /* FLUXABSTRACT_HPP */
