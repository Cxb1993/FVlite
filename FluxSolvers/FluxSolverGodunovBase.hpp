// Copied from old code. Exact Riemann problem solver.
// Intending to use this to build a godunov solver.
// This could be used to approximate the boundary flux in the cut cell method
//
// ==========================================================================


// ExactSolver.hpp
//
// Finite volume Riemann problem exact solver.
// Designed to solve Toro's test problems. Not general case.
//
// Author: Liam Pattinson
// Date: Feb 19 2016

/* Notes on the Euler equations:
 *
 * U_t + F(U)_x = 0
 *     
 * U = [ rho, rho*u, E ]
 * F = [ rho*u, rho*u^2+p, u(E+p) ]
 *
 * U defines the vector of conserved quantities
 *
 * The primitive (physical) variables are:
 *
 * P = [ rho, u, p]
 */

#ifndef EULEREXACT_HPP
#define EULEREXACT_HPP

#define gam 1.4

#include <cstdlib>
#include <cmath>

#include <iostream>

#include "Vectors/Grid.hpp"
#include "Timer/Timer.hpp"

namespace Euler{

enum WAVE{
    SHOCK,
    RARE
};

class ExactSolver{

private:

    Grid* pGrid;
    Timer* pTimer;

    double x0;

    double rhoL, uL, pL, aL;
    double rhoR, uR, pR, aR;
    double dU;

    double p_min, p_max;
    double f_min, f_max;

    double AL, AR, BL, BR;

    double tol;

    WAVE waveL;
    WAVE waveR;

    double p_star;
    double u_star;
    double rhoL_star;
    double rhoR_star;

    double SHL, STL;
    double SHR, STR;


public:


    ExactSolver( Grid* pGrid, Timer* pTimer, double x0, double rhoL, double uL, double pL, double rhoR, double uR, double pR, double tol=1e-6);
    ~ExactSolver();

    void exec();
    
    void solve_p_star( double p0);
    void solve_u_star();
    void determine_waves();
    void solve_rhoL_star();
    void solve_rhoR_star();
    void determine_wave_speeds();

    StateVector L();
    StateVector R();
    StateVector L_star();
    StateVector R_star();
    StateVector L_fan( double s);
    StateVector R_fan( double s);

    double f( double p);
    double df( double p);

    double fL( double p);
    double fR( double p);
    double dfL( double p);
    double dfR( double p);

};

ExactSolver::ExactSolver( Grid* pGrid, Timer* pTimer, double x0, double rhoL, double uL, double pL, double rhoR, double uR, double pR, double tol) :
        pGrid(pGrid),
        pTimer(pTimer),
        x0(x0),
        rhoL(rhoL),
        uL(uL),
        pL(pL),
        rhoR(rhoR),
        uR(uR),
        pR(pR),
        dU(uR-uL),
        tol(tol){

    StateVector StateL(rhoL,uL,pL);
    StateVector StateR(rhoR,uR,pR);
    aL = StateL.a();
    aR = StateR.a();

    p_min = ( pL < pR ) ? pL : pR;
    p_max = ( pL < pR ) ? pR : pL;
    f_min = f(p_min);
    f_max = f(p_max);

    AL = 2./((gam+1.)*rhoL);
    AR = 2./((gam+1.)*rhoR);
    BL = pL*(gam-1.)/(gam+1.);
    BR = pR*(gam-1.)/(gam+1.);

    //solve_p_star(0.5*(pL+pR));
    solve_p_star(tol);
    solve_u_star();
    determine_waves();    
    solve_rhoL_star();
    solve_rhoR_star();
    determine_wave_speeds();

}

ExactSolver::~ExactSolver(){
}

void ExactSolver::exec(){
    double x, t, s;
    StateVector State;
    t = pTimer->tmax();
    for( int ii=pGrid->start(); ii<pGrid->end(); ii++){
        x = pGrid->x(ii);
        s = (x-x0) / t;
        if( s < u_star){
            // Left of contact
            if( waveL == SHOCK){
                if( s < STL){
                    State = L();
                } else {
                    State = L_star();
                }
            } else {
                if( s < SHL ){
                    State = L();
                } else if ( s < STL ){
                    State = L_fan(s);
                } else {
                    State = L_star();
                }
            }
        } else {
            // Right of contact
            if( waveR == SHOCK){
                if( s > STR){
                    State = R();
                } else {
                    State = R_star();
                }
            } else {
                if( s > SHR ){
                    State = R();
                } else if ( s > STR ){
                    State = R_fan(s);
                } else {
                    State = R_star();
                }
            }
        }
        pGrid->state(ii) = State;
    }
    return;
}

void ExactSolver::solve_p_star(double p0){
    // Uses Newton-Raphson method
    double p_star_old=p0;
    double err_rel;
    int its=0;
    int max_its = 1000;
    while(its<max_its){
        p_star = p_star_old - f(p_star_old)/df(p_star_old);
        err_rel = 2.*fabs(p_star-p_star_old)/(p_star+p_star_old);
        if( err_rel < tol) break;
        p_star_old = p_star;
        its++;
    };
    if(its>=max_its){
        std::cerr << "ERROR: PRESSURE CALCULATOR EXCEEDED MAXITS" << std::endl;
        exit(EXIT_FAILURE);
    }
    return;
}

void ExactSolver::solve_u_star(){
    u_star = 0.5*(uL+uR+fR(p_star)-fL(p_star));
    return;
}

void ExactSolver::determine_waves(){
    waveL = (p_star > pL) ? SHOCK : RARE;
    waveR = (p_star > pR) ? SHOCK : RARE;
    return;
}
    

void ExactSolver::solve_rhoL_star(){
    double numer, denom;
    if( waveL == SHOCK){
        numer = p_star*(gam+1) + pL*(gam-1);
        denom = p_star*(gam-1) + pL*(gam+1);
        rhoL_star = rhoL*numer/denom;
    } else {
        rhoL_star = rhoL*pow(p_star/pL, 1./gam);
    }
    return;
}

void ExactSolver::solve_rhoR_star(){
    double numer, denom;
    if( waveR == SHOCK){
        numer = p_star*(gam+1) + pR*(gam-1);
        denom = p_star*(gam-1) + pR*(gam+1);
        rhoR_star = rhoR*numer/denom;
    } else {
        rhoR_star = rhoR*pow(p_star/pR, 1./gam);
    }
    return;
}

void ExactSolver::determine_wave_speeds(){
    double aL_star, aR_star;
    if( waveL == SHOCK){
        STL = uL - aL*sqrt((p_star*(gam+1))/(2*gam*pL)+(gam-1)/(2*gam));
        SHL = STL;
    } else {
        aL_star = aL*pow(p_star/pL,(gam-1)/(2*gam));
        STL = u_star - aL_star;
        SHL = uL - aL;
    }
    if( waveR == SHOCK){
        STR = uR + aR*sqrt((p_star*(gam+1))/(2*gam*pR)+(gam-1)/(2*gam));
        SHR = STR;
    } else {
        aR_star = aR*pow(p_star/pR,(gam-1)/(2*gam));
        STR = u_star + aR_star;
        SHR = uR + aR;
    }
    return;
}

StateVector ExactSolver::L(){
    StateVector result( rhoL, uL, pL);
    return result;
}

StateVector ExactSolver::R(){
    StateVector result( rhoR, uR, pR);
    return result;
}

StateVector ExactSolver::L_star(){
    StateVector result( rhoL_star, u_star, p_star);
    return result;
}

StateVector ExactSolver::R_star(){
    StateVector result( rhoR_star, u_star, p_star);
    return result;
}

StateVector ExactSolver::L_fan( double s){
    double rhoL_fan, uL_fan, pL_fan;
    rhoL_fan = rhoL*pow( 2./(gam+1) + (uL-s)*(gam-1)/(aL*(gam+1)) ,2./(gam-1));
    uL_fan   = (aL + 0.5*uL*(gam-1) + s)*2./(gam+1);
    pL_fan   = pL*pow( 2./(gam+1) + (uL-s)*(gam-1)/(aL*(gam+1)) ,(2.*gam)/(gam-1));
    StateVector result( rhoL_fan, uL_fan, pL_fan);
    return result;
}

StateVector ExactSolver::R_fan( double s){
    double rhoR_fan, uR_fan, pR_fan;
    rhoR_fan = rhoR*pow( 2./(gam+1) - (uR-s)*(gam-1)/(aR*(gam+1)) ,2./(gam-1));
    uR_fan   = (-aR + 0.5*uR*(gam-1) + s)*2./(gam+1);
    pR_fan   = pR*pow( 2./(gam+1) - (uR-s)*(gam-1)/(aR*(gam+1)) ,(2.*gam)/(gam-1));
    StateVector result( rhoR_fan, uR_fan, pR_fan);
    return result;
}

double ExactSolver::f( double p){
    return fL(p) + fR(p) + dU;
}

double ExactSolver::df( double p){
    return dfL(p) + dfR(p);
}

double ExactSolver::fL( double p){
    double result;
    if( p > pL){
        // Left Shock
        result = (p-pL)*sqrt(AL/(p+BL));
    } else {
        // Left rarefaction
        result = (2.*aL/(gam-1))*(pow(p/pL,(gam-1.)/(2*gam))-1.);
    }
    return result;
}

double ExactSolver::fR( double p){
    double result;
    if( p > pR){
        // Right Shock
        result = (p-pR)*sqrt(AR/(p+BR));
    } else {
        // Left rarefaction
        result = (2.*aR/(gam-1))*(pow(p/pR,(gam-1.)/(2*gam))-1.);
    }
    return result;
}

double ExactSolver::dfL( double p){
    double result;
    if( p > pL){
        // Left Shock
        result = sqrt(AL/(p+BL))*(1.-(p-pL)/(2.*(BL+p)));
    } else {
        // Left rarefaction
        result = (1./(rhoL*aL))*pow(p/pL,-1.*(gam+1.)/(2*gam));
    }
    return result;
}

double ExactSolver::dfR( double p){
    double result;
    if( p > pR){
        // Left Shock
        result = sqrt(AR/(p+BR))*(1.-(p-pR)/(2.*(BR+p)));
    } else {
        // Left rarefaction
        result = (1./(rhoR*aR))*pow(p/pR,-1.*(gam+1.)/(2*gam));
    }
    return result;
}

}// Namespace closure
#endif /* EULEREXACT_HPP */
