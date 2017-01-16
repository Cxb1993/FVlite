// FVlite.hpp
//
// Defines top-level class which performs initial setup,
// initialises the key components, and provides a simple
// interface to them.

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <libconfig.h++>

#include "Grid/Grid.hpp"
#include "Timer/Timer.hpp"
#include "Operators/Operators.hpp"
#include "Output/Output.hpp"

using std::string;
using libconfig::Config;
using libconfig::Setting;

namespace FVlite{

class Controller{

private:

    Grid   mGrid;
    Timer  mTimer;
    Output mOutput;

    CompositeOperator<Operator> mSolver;

public:

//    Controller(){}
    Controller( Grid& grid, Config& cfg) : mGrid(grid) { init(cfg);}

    void init( Config& cfg);

    bool is_complete();
    void advance();
    void solve();
    void exec();
//    void check_grid();

    // PRINT FUNCTIONS, SHOULD BE DEPRECATED
//    void printData(std::ofstream& file);
//    void printGeometry();
//    void printLevelSetVertices();
};

void Controller::init( Config& cfg){ 

    std::cout << "Cells: [" << mGrid.num_cells(0) << ',' << mGrid.num_cells(1) << ']' << std::endl;
    std::cout << "Length: [" << mGrid.length(0) << ',' << mGrid.length(1) << ']' << std::endl;

    std::cout << "Begin Building Solver" << std::endl;


    // Set up timer
    std::cout << "Building timer..." << std::endl;
    Setting& timerCfg = cfg.lookup("Timing");
    mTimer.init(timerCfg);

    // Output
    std::cout << "Setting up output..." << std::endl;
    Setting& outputCfg = cfg.lookup("Output");
    mOutput.init(&mGrid,&mTimer,outputCfg);

    // Set up Solver
    std::cout << "Building solver..." << std::endl;
    Setting& solverCfg = cfg.lookup("Solver");
    // Start process with timer calibration
    Operator* pOperator;
    pOperator = OperatorFactory.create("TimerCalibrate");
    pOperator->init(solverCfg);
    mSolver.add_element( pOperator);
    // Find all user-specified solvers, wrap in order
    string operatorType;
    int nOperators = solverCfg.getLength();
    for( int count=0; count<nOperators; count++){
        Setting& thisCfg = solverCfg[count];
        operatorType = thisCfg.lookup("type").c_str();
        pOperator = OperatorFactory.create(operatorType);
        pOperator->init(thisCfg);
        mSolver.add_element( pOperator);
    }
    // Finalise with timer increment
    pOperator = OperatorFactory.create("TimerIncrement");
    pOperator->init(solverCfg);
    mSolver.add_element(pOperator);

    // Initialise
    std::cout << "Building initialiser..." << std::endl;
    Setting& initCfg = cfg.lookup("Initialisation");
    // Build objects on grid
    OperatorInitialisationManager init_manager;
    init_manager.init(initCfg);
    std::cout << "Initialising..." << std::endl;
    init_manager.exec( mGrid, mTimer);
    init_manager.setup_boundary_geometry(mGrid);
    // Build and use temporary TimerCalibrate Operator
    OperatorTimerCalibrate init_calibration;
    init_calibration.exec( mGrid, mTimer);

    std::cout << "Solver built successfully" << std::endl;
    mOutput.prod();
    return;
}

bool Controller::is_complete(){
    return mTimer.is_complete();
}

void Controller::advance(){
    // Print current time to screen
    std::cout << "\r| Time: "
        << std::setw(16) << std::setprecision(10) << std::right << mTimer.t() 
        << " | Tmax: "
        << std::setw(16) << std::setprecision(10) << std::right << mTimer.tmax()
        << " |"
        << std::flush;
    // Execute main solver
    mSolver.exec( mGrid, mTimer);
    // Request IO
    mOutput.prod();
    return;
}

void Controller::solve(){
    while( !mTimer.is_complete()){
        advance();
    }
    return;
}

void Controller::exec(){
    solve();
    mOutput.print();
    return;
}
/*
void Controller::check_grid(){
    int startX = mGrid.startX();
    int startY = mGrid.startY();
    int endX = mGrid.endX();
    int endY = mGrid.endY();
    StateVector State;
    FluxVector Flux;
    bool abort = false;
    for( int jj=startY; jj<endY; jj++){
        for( int ii=startX; ii<endX; ii++){
            State = mGrid.state(ii,jj);
            Flux  = mGrid.flux(ii,jj);
            if( State.isnan() ){
                std::cerr << "State("<<ii<<","<<jj<<") contains NaN" << std::endl;
                abort = true;
            }
            if( Flux.isnan() ){
                std::cerr << "Flux("<<ii<<","<<jj<<") contains NaN" << std::endl;
                abort = true;
            }
        }
    }
    if( abort ){
        std::cerr << "NaNs found in mGrid. Aborting." << std::endl;
        exit(EXIT_FAILURE);
    }
    return;
}


// ADDITIONAL PRINT FUNCTIONS
// Should be deprecated!


void Controller::printData(std::ofstream& file){
#ifdef MAXWELL
    file << "# x y Ex Ey Ez Hx Hy Hz LevelSet" << std::endl;
#endif
#ifdef EULER
    file << "# x y rho rhoUx rhoUy E levelset" << std::endl;
#endif
    StateVector State;
    for( int ii=mGrid.startX(); ii<mGrid.endX(); ii++){
        for( int jj=mGrid.startY(); jj<mGrid.endY(); jj++){
            State = mGrid.state(ii,jj);
            file << mGrid.x(ii)  << '\t' << mGrid.y(jj)  << '\t';
            for( unsigned int kk=0; kk<State.size(); kk++){
                file << State[kk] << '\t';
            }
            file << (*mGrid.levelset())(ii,jj) << std::endl;
        }
        file << std::endl;
    }
    return;
}

void Controller::printGeometry(){
    std::ofstream file("geometry.dat");
    file << "# x y levelset betaL betaR betaT betaB alpha Xb[0] Xb[1] Xb[2] Nb[0] Nb[1] Nb[2]" << std::endl;
    int startX = mGrid.startX();
    int startY = mGrid.startY();
    int endX = mGrid.endX();
    int endY = mGrid.endY();
    BoundaryGeometry geometry;
    for( int ii=startX; ii<endX; ii++){
        for( int jj=startY; jj<endY; jj++){
            geometry = mGrid.boundary(ii,jj);
            file << mGrid.x(ii)  << '\t' << mGrid.y(jj)  << '\t';
            file << (*mGrid.levelset())(ii,jj) << '\t'; 
            file << geometry.betaL() << '\t';
            file << geometry.betaR() << '\t';
            file << geometry.betaT() << '\t';
            file << geometry.betaB() << '\t';
            file << geometry.alpha() << '\t';
            file << (geometry.Xb())[0] << '\t';
            file << (geometry.Xb())[1] << '\t';
            file << (geometry.Xb())[2] << '\t';
            file << (geometry.Nb())[0] << '\t';
            file << (geometry.Nb())[1] << '\t';
            file << (geometry.Nb())[2] << '\t';
            file << std::endl;
        }
        file << std::endl;
    }
    return;
}

void Controller::printLevelSetVertices(){
    std::ofstream file("vertices.dat");
    file << "# x y levelset" << std::endl;
    int sizeX = mGrid.sizeX();
    int sizeY = mGrid.sizeY();
    double x, y;
    double dx = mGrid.dx();
    double dy = mGrid.dy();
    for( int ii=1; ii<sizeX; ii++){
        x = mGrid.x(ii) - 0.5*dx;
        for( int jj=1; jj<sizeY; jj++){
            y = mGrid.y(jj) - 0.5*dy;
            file << x  << '\t' << y  << '\t';
            file << mGrid.levelset()->interpolate(x,y) << std::endl; 
        }
        file << std::endl;
    }
    return;
}
*/
}// Namespace closure
#endif /* CONTROLLER_HPP */
