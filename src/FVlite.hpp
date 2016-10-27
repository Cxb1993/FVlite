// FVlite.hpp
//
// Defines top-level class which performs initial setup,
// initialises the key components, and provides a simple
// interface to them.

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <string>
#include <iostream>
#include <fstream>

#include <libconfig.h++>

#include "Grid/Grid.hpp"
#include "Timer/Timer.hpp"
#include "Solvers/Solvers.hpp"
#include "Initialisation/InitialisationManager.hpp"
#include "Output/Output.hpp"

using std::string;
using libconfig::Config;
using libconfig::Setting;

namespace FVlite{

class Controller{

private:

    Grid*             mpGrid;
    Timer*            mpTimer;
    Solver*           mpSolver;
    Output*           mpOutput;

public:

    Controller(){}
    Controller( Config& cfg){ init(cfg);}
    ~Controller();

    void init( Config& cfg);

    bool is_complete();
    void advance();
    void solve();
    void exec();
    void check_grid();

    // PRINT FUNCTIONS, SHOULD BE DEPRECATED
    void printData(std::ofstream& file);
    void printGeometry();
    void printLevelSetVertices();
};

Controller::~Controller(){
  delete mpGrid;
  delete mpTimer;
  delete mpSolver;
  delete mpOutput;
}

void Controller::init( Config& cfg){ 

    std::cout << "Begin Building Solver" << std::endl;

    // Set up grid
    std::cout << "Building grid.." << std::endl;
    Setting& gridCfg = cfg.lookup("Grid");
    mpGrid = new Grid;
    mpGrid->init(gridCfg);

    // Set up timer
    std::cout << "Building timer..." << std::endl;
    Setting& timerCfg = cfg.lookup("Timing");
    mpTimer = new Timer;
    mpTimer->init(mpGrid,timerCfg);
    mpTimer->calibrate_timestep();

    // Output
    std::cout << "Setting up output..." << std::endl;
    Setting& outputCfg = cfg.lookup("Output");
    mpOutput = new Output;
    mpOutput->init(mpGrid,mpTimer,outputCfg);

    // Set up Solver
    std::cout << "Building solver..." << std::endl;
    Setting& solverCfg = cfg.lookup("Solver");
    int nSolvers = solverCfg.getLength();
    Solver* pSolver;
    mpSolver = new Solver;
    mpSolver->init( mpGrid, mpTimer, solverCfg);
    string solverType;
    for( int count=0; count<nSolvers; count++){
        Setting& thisCfg = solverCfg[count];
        solverType = thisCfg.lookup("type").c_str();
        pSolver = SolverFactory.create(solverType);
        pSolver->init( mpGrid, mpTimer, thisCfg, mpSolver);
        mpSolver = pSolver;
    }

    // Initialise
    std::cout << "Building initialiser..." << std::endl;
    Setting& initCfg = cfg.lookup("Initialisation");
    InitialisationManager* pImanager = new InitialisationManager;
    pImanager->init(mpGrid,initCfg);
    pImanager->exec();
    pImanager->setup_boundary_geometry();
    delete pImanager;

    std::cout << "Solver built successfully" << std::endl;
    mpOutput->prod();
#ifdef DEBUG
    pOutput->print_geometry();
#endif

    return;
}

bool Controller::is_complete(){
    return mpTimer->is_complete();
}

void Controller::advance(){
    // Calibrate time step using largest speed on the grid
#ifdef EULER
    mpTimer->calibrate_timestep();
#endif

    // Tell FVMsolver that a new time step is occurring
    mpSolver->newTimeStep();

    // Print current time to screen
    std::cout << "\rTime: " << mpTimer->t()
#ifdef DEBUG 
        << std::endl;
#else
        << std::flush;
#endif

    mpSolver->exec();
/*    char dim;
    (void) t;

    dim = 'x';
    mpBoundaryManager->exec(dim,t);
    mpSolver->exec(dim,dt); 

#ifdef DEBUG
    check_grid();
#endif

    dim = 'y';
    mpBoundaryManager->exec(dim,t);
    mpSolver->exec(dim,dt);

#ifdef DEBUG
    check_grid();
#endif
*/
//    dim = 'x';
//    pBmanager->exec(dim,t,dt);
//    pFVM->exec(dim,0.5*dt);
//
//#ifdef DEBUG
//    checkGrid();
//#endif

    // Increment time
    mpTimer->advance();
    mpOutput->prod();
    return;
}

void Controller::solve(){
    while( !mpTimer->is_complete()){
        advance();
    }
    return;
}

void Controller::exec(){
    solve();
    mpOutput->print();
    return;
}

void Controller::check_grid(){
    int startX = mpGrid->startX();
    int startY = mpGrid->startY();
    int endX = mpGrid->endX();
    int endY = mpGrid->endY();
    StateVector State;
    FluxVector Flux;
    bool abort = false;
    for( int jj=startY; jj<endY; jj++){
        for( int ii=startX; ii<endX; ii++){
            State = mpGrid->state(ii,jj);
            Flux  = mpGrid->flux(ii,jj);
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
        std::cerr << "NaNs found in grid. Aborting." << std::endl;
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
    for( int ii=mpGrid->startX(); ii<mpGrid->endX(); ii++){
        for( int jj=mpGrid->startY(); jj<mpGrid->endY(); jj++){
            State = mpGrid->state(ii,jj);
            file << mpGrid->x(ii)  << '\t' << mpGrid->y(jj)  << '\t';
            for( unsigned int kk=0; kk<State.size(); kk++){
                file << State[kk] << '\t';
            }
            file << (*mpGrid->levelset())(ii,jj) << std::endl;
        }
        file << std::endl;
    }
    return;
}

void Controller::printGeometry(){
    std::ofstream file("geometry.dat");
    file << "# x y levelset betaL betaR betaT betaB alpha Xb[0] Xb[1] Xb[2] Nb[0] Nb[1] Nb[2]" << std::endl;
    int startX = mpGrid->startX();
    int startY = mpGrid->startY();
    int endX = mpGrid->endX();
    int endY = mpGrid->endY();
    BoundaryGeometry geometry;
    for( int ii=startX; ii<endX; ii++){
        for( int jj=startY; jj<endY; jj++){
            geometry = mpGrid->boundary(ii,jj);
            file << mpGrid->x(ii)  << '\t' << mpGrid->y(jj)  << '\t';
            file << (*mpGrid->levelset())(ii,jj) << '\t'; 
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
    int sizeX = mpGrid->sizeX();
    int sizeY = mpGrid->sizeY();
    double x, y;
    double dx = mpGrid->dx();
    double dy = mpGrid->dy();
    for( int ii=1; ii<sizeX; ii++){
        x = mpGrid->x(ii) - 0.5*dx;
        for( int jj=1; jj<sizeY; jj++){
            y = mpGrid->y(jj) - 0.5*dy;
            file << x  << '\t' << y  << '\t';
            file << mpGrid->levelset()->interpolate(x,y) << std::endl; 
        }
        file << std::endl;
    }
    return;
}

}// Namespace closure
#endif /* CONTROLLER_HPP */
