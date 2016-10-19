// Test file for HLLC functions.

#include<cstdlib>
#include<iostream>
#include<iomanip>
#include"HLLCfunctions.hpp"

using namespace FVlite;
using namespace std;

int main(void){

    MathVector<4> Lprim,Rprim;
    StateVector L,R,H;
    Vector3 S;
    FluxVector F;
    char d;
    int p;

    Lprim[0] = 1.0; Rprim[0] = 1.0;
    Lprim[1] = 1.0; Rprim[1] = 1.0;
    Lprim[2] = 0.0; Rprim[2] = 0.0;
    Lprim[3] = 1.0; Rprim[3] =  1.0;

    d = 'y';
    p = 20;
    L.set(Lprim);
    R.set(Rprim);
    S = HLLC::getWaveSpeeds(d,L,R);
    H = HLLC::getHLLCstate(d,L,R,S);
    F = HLLC::getHLLCflux(d,L,R,H,S);

    cout << "L STATE:" << endl;
    cout << setprecision(p) << "( " << L.rho() << ", " << L.ux() << ", " << L.uy() << ", " << L.p() << ")" << endl;
    cout << "R STATE:" << endl;
    cout << setprecision(p) << "( " << R.rho() << ", " << R.ux() << ", " << R.uy() << ", " << R.p() << ")" << endl;
    cout << "HLLC STATE:" << endl;
    cout << setprecision(p) << "( " << H.rho() << ", " << H.ux() << ", " << H.uy() << ", " << H.p() << ")" << endl;
    cout << "FLUX:" << endl;
    cout << setprecision(p) << "( " << F[0] << ", " << F[1] << ", " << F[2] << ", " << F[3] << ")" << endl;

    return EXIT_SUCCESS;
}

