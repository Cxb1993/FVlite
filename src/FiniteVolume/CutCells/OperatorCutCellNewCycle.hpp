// OperatorCutCellNewCycle.hpp
//
// To be called at the start of each time step, if using cut cells.
// Sets up reference states.

#ifndef OPERATORCUTCELLNEWCYCLE_HPP
#define OPERATORCUTCELLNEWCYCLE_HPP

#include <libconfig.h++>

#include "Operators/Operator.hpp"
#include "FiniteVolume/FluxSolvers/FluxCalculators/FluxCalculators.hpp"
#include "Boundaries/BoundaryFunctions.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class OperatorCutCellNewCycle : public Operator {
public:
    void exec();
};

// Register with factory

REGISTER( Operator, CutCellNewCycle)

// Function definitions

void OperatorCutCellNewCycle::exec(){
#ifdef DEBUG
    std::cout << "NEW TIME STEP" << std::endl;
#endif
    int startX=mpGrid->startX();
    int startY=mpGrid->startY();
    int endX=mpGrid->endX();
    int endY=mpGrid->endY();
    StateVector State;
    StateVector BoundaryState;
#ifdef MAXWELL
    StateVector Rotated;
#endif
#ifdef EULER
    Vector3 Velocity;
    Vector3 VelocityR;
    Vector3 WaveSpeeds;
    Velocity[2]=0; VelocityR[2]=0; 
#endif
    Vector3 Normal;
    Normal[2]=0;
    double angle;
    BoundaryGeometry Boundary;
    for( int jj=startY; jj<endY; jj++){
        for( int ii=startX; ii<endX; ii++){
            Boundary = mpGrid->boundary(ii,jj);
            if(Boundary.alpha() == 1. || Boundary.alpha() == 0.) continue;
            
            State = mpGrid->state(ii,jj);

            // Step I
            // Find rotation matrix to convert velocity to tangential/normal frame
            Normal = Boundary.Nb();
            angle = atan2( Normal[1], Normal[0]);

            // Step II
            // Rotate StateVector into that frame
#ifdef EULER
            Velocity = State.getVelocity();
            VelocityR[0] =  Velocity[0]*cos(angle) + Velocity[1]*sin(angle);
            VelocityR[1] =  -Velocity[0]*sin(angle) + Velocity[1]*cos(angle);
            State.set(State.rho(),VelocityR[0],VelocityR[1],State.p());
#endif
#ifdef MAXWELL
            Rotated.Ex() = State.Ex()*cos(angle) + State.Ey()*sin(angle);
            Rotated.Hx() = State.Hx()*cos(angle) + State.Hy()*sin(angle);
            Rotated.Ey() = -State.Ex()*sin(angle) + State.Ey()*cos(angle);
            Rotated.Hy() = -State.Hx()*sin(angle) + State.Hy()*cos(angle);
            Rotated.Ez() = State.Ez();
            Rotated.Hz() = State.Hz();
            State = Rotated;
#endif
            // Step III
            // Get appropriate boundary state
            // (Behaviour for Maxwell's equations contained in boundary function)
            BoundaryState = Boundary::Reflective( State, 'x');

            /* For parallel flow test
            if(fabs(State.ux())>1e-10){
                std::cout << "Normal velocity non-zero: " << State.ux() << std::endl;
                std::cout << "Angle: " << angle*180/M_PI << std::endl;
                std::cout << "Velocity: ( " << Velocity[0] << ", " << Velocity[1] << ")" << std::endl;
                std::cout << "VelocityR: ( " << VelocityR[0] << ", " << VelocityR[1] << ")" << std::endl;
            } 
            */

            // Step IV
            // Euler: Solve Riemann problem at boundary
            // Maxwell: I have no idea!
#ifdef EULER
            WaveSpeeds = HLLC::getWaveSpeeds('x',State,BoundaryState);
            State = HLLC::getHLLCstate('x',State,BoundaryState,WaveSpeeds[0],WaveSpeeds[1],WaveSpeeds[2]);
#endif
#ifdef MAXWELL
//            State = 0.5*(BoundaryState+State); //?????
            State = BoundaryState;
#endif

            // Step V
            // Rotate back to standard frame
            // NOTE: This method tends to introduce unsual behaviour in the tangential velocity.
            // Now manually setting it to be maintained from original state.
#ifdef EULER
            Velocity = State.getVelocity();
            VelocityR[0] = Velocity[0]*cos(angle) - Velocity[1]*sin(angle);
            VelocityR[1] = Velocity[0]*sin(angle) + Velocity[1]*cos(angle);
            State.set(State.rho(),VelocityR[0],VelocityR[1],State.p());
#endif
#ifdef MAXWELL
            Rotated.Ex() = State.Ex()*cos(angle) - State.Ey()*sin(angle);
            Rotated.Hx() = State.Hx()*cos(angle) - State.Hy()*sin(angle);
            Rotated.Ey() = State.Ex()*sin(angle) + State.Ey()*cos(angle);
            Rotated.Hy() = State.Hx()*sin(angle) + State.Hy()*cos(angle);
            Rotated.Ez() = State.Ez();
            Rotated.Hz() = State.Hz();
            State = Rotated;
#endif

            // Step V alt
            // Retain only tangential component of velocity at interface
            //Velocity = Velocity - (Velocity*Normal)*Normal;
            //State.set(State.rho(),Velocity[0],Velocity[1],State.p());

            // Step VI
            // Store result in reference state
            mpGrid->state_ref(ii,jj) = State;
        }
    }

    return;
}

} // namespace closure
#endif /* OPERATORCUTCELLNEWCYCLE_HPP */
