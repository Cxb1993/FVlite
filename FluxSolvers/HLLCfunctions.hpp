// HLLCfunctions.hpp
//
// Contains functions used to compute wavespeeds, states and fluxes using the HLLC scheme
// Functions are compartmentalised here to allow access from other parts of the code.
// In particular, the Klein cut cell method may make use of a first order HLLC flux.

#ifndef HLLCFUNCTIONS_HPP
#define HLLCFUNCTIONS_HPP

#include "Vectors/StateVector.hpp"
#include "Vectors/FluxVector.hpp"
#include "Vectors/Vector3.hpp"
#include "constants.hpp"

#include <cstdlib>
#include <cmath>
#include <ctgmath>
#include <iostream>

namespace FVlite{
namespace HLLC{

Vector3 getWaveSpeeds( char dim, const StateVector& UL, const StateVector& UR);
StateVector getHLLCstate( char dim, const StateVector& UL, const StateVector& UR, const Vector3& WaveSpeeds);
StateVector getHLLCstate( char dim, const StateVector& UL, const StateVector& UR, double SL, double SR, double Sstar);
FluxVector getHLLCflux( char dim, const StateVector& UL, const StateVector& UR, const StateVector& HLLCstate, const Vector3& WaveSpeeds);
FluxVector getHLLCflux( char dim, const StateVector& UL, const StateVector& UR, const StateVector& HLLCstate, double SL, double SR, double Sstar);


Vector3 getWaveSpeeds( char dim, const StateVector& StateL, const StateVector& StateR){

    // Setup
    const double gam = c_gamma_ideal;
    double rho_L = StateL.rho();
    double rho_R = StateR.rho();
    double p_L   = StateL.p();
    double p_R   = StateR.p();
    double a_L   = StateL.a();
    double a_R   = StateR.a();
    double u_L, u_R;
    switch(dim){
        case 'x' :
            u_L = StateL.ux();
            u_R = StateR.ux();
            break;
        case 'y' :
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
    double delta_L = S_L - u_L;
    double delta_R = S_R - u_R;
    double S_star = (p_R - p_L + rho_L*u_L*delta_L - rho_R*u_R*delta_R) / (rho_L*delta_L - rho_R*delta_R);

    // Step III: bundle into Vector3

    Vector3 result;
    result[0] = S_L;
    result[1] = S_R;
    result[2] = S_star;

    return result;
}

StateVector getHLLCstate( char dim, const StateVector& StateL, const StateVector& StateR, const Vector3& WaveSpeeds){
    return getHLLCstate(dim,StateL,StateR,WaveSpeeds[0],WaveSpeeds[1],WaveSpeeds[2]);
}

StateVector getHLLCstate( char dim, const StateVector& StateL, const StateVector& StateR, double S_L, double S_R, double S_star){

    // Get easy cases out of the way

    if( S_L >= 0.) return StateL;
    if( S_R <= 0.) return StateR;

    // Prepare for the harder cases
    double rho_L = StateL.rho();
    double rho_R = StateR.rho();
    double p_L   = StateL.p();
    double p_R   = StateR.p();
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
            std::cerr << "ERROR, INCORRECT DIM, HLLC STATE SOLVER" << std::endl;
            exit(EXIT_FAILURE);
    } 
    double delta_L = S_L - u_L;
    double delta_R = S_R - u_R;

    StateVector State_star;

    if( S_star >= 0){ // U_star_L
        State_star = StateL/rho_L;
        State_star[speed_idx] = S_star;
        State_star[StateVector::size()-1] = StateL.E()/rho_L + (S_star-u_L)*(S_star + p_L/(rho_L*delta_L));
        State_star *= rho_L*delta_L/(S_L-S_star);
    } else { // F_star_R
        State_star = StateR/rho_R;
        State_star[speed_idx] = S_star;
        State_star[StateVector::size()-1] = StateR.E()/rho_R + (S_star-u_R)*(S_star + p_R/(rho_R*delta_R));
        State_star *= rho_R*delta_R/(S_R-S_star);
    }

    return State_star;
}

FluxVector getHLLCflux( char dim, const StateVector& UL, const StateVector& UR, const StateVector& StateHLLC, const Vector3& WaveSpeeds){
    return getHLLCflux( dim,UL,UR,StateHLLC,WaveSpeeds[0],WaveSpeeds[1],WaveSpeeds[2]);
}

FluxVector getHLLCflux( char dim, const StateVector& StateL, const StateVector& StateR, const StateVector& State_HLLC, double S_L, double S_R, double S_star){
    FluxVector Flux;
    FluxVector Flux_HLLC;

    if( S_L >= 0.){
        Flux_HLLC.set(State_HLLC,dim);
        return Flux_HLLC;
    }

    if( S_R <= 0.){
        Flux_HLLC.set(State_HLLC,dim);
        return Flux_HLLC;
    }


    if( S_star >= 0){ // F_star_L
        Flux.set(StateL,dim);
        Flux_HLLC = Flux + S_L*(State_HLLC - StateL);
    } else { // F_star_R
        Flux.set(StateR,dim);
        Flux_HLLC = Flux + S_R*(State_HLLC - StateR);
    }
    return Flux_HLLC;
}


}// namespace closure, HLLC
}// namespace closure, FVlite
#endif /* HLLCFUNCTIONS_HPP */
