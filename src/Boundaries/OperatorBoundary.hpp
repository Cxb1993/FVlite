// OperatorBoundary.hpp
//
// Boundary update operator.

#ifndef OPERATORBOUNDARY_HPP
#define OPERATORBOUNDARY_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <libconfig.h++>

#include "Grid/Grid.hpp"
#include "Boundaries/BoundaryModules.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

enum BOUNDARY_REGION{
    LEFTWALL,
    RIGHTWALL,
    UPPERWALL,
    LOWERWALL,
    INTERNAL
};

class OperatorBoundary : public Operator, public Visitable<OperatorBoundary>{

protected:

    BoundaryModule* mpBmod;

    char mDim;
    BOUNDARY_REGION mREGION;

public:

    OperatorBoundary(){}
    virtual ~OperatorBoundary();

    virtual void init( char dim, Setting& cfg);
    void exec( Grid& grid, double t);
};

// Register with factory

REGISTER( Operator, Boundary)

// Function definitions

OperatorBoundary::~OperatorBoundary(){
    delete mpBmod;
}

void OperatorBoundary::init(char dim, Setting& cfg){
    mDim = dim;
    string region = cfg.lookup("region");
    if( region == "leftwall")  mREGION = LEFTWALL;
    if( region == "rightwall") mREGION = RIGHTWALL;
    if( region == "upperwall") mREGION = UPPERWALL;
    if( region == "lowerwall") mREGION = LOWERWALL;
    if( region == "internal")  mREGION = INTERNAL;
    string modType = cfg.lookup("type");
    mpBmod = BoundaryModuleFactory.create( modType);
    mpBmod->init(cfg);
    return;
}

void OperatorBoundary::exec( Grid& grid, double t){
    int bound  = grid.bound();
    int startX = grid.startX();
    int startY = grid.startY();
    int endX   = grid.endX();
    int endY   = grid.endY();
    double levelset;

    switch( mREGION ){
        case LEFTWALL :
            if( mDim != 'x') break;
            for( int ii=0; ii<bound; ii++){
                for( int jj=startY; jj<endY; jj++){
                    grid.state(startX-1-ii,jj) = mpBmod->getBoundary(grid.state(startX+ii,jj),mDim,t);
                }
            }
            break;
        case RIGHTWALL :
            if( mDim != 'x') break;
            for( int ii=0; ii<bound; ii++){
                for( int jj=startY; jj<endY; jj++){
                    grid.state(endX+ii,jj) = mpBmod->getBoundary(grid.state(endX-1-ii,jj),mDim,t);
                }
            }
            break;
        case LOWERWALL :
            if( mDim != 'y') break;
            for( int ii=startX; ii<endX; ii++){
                for( int jj=0; jj<bound; jj++){
                    grid.state(ii,startY-1-jj) = mpBmod->getBoundary(grid.state(ii,startY+jj),mDim,t);
                }
            }
            break;
        case UPPERWALL :
            if( mDim != 'y') break;
            for( int ii=startX; ii<endX; ii++){
                for( int jj=0; jj<bound; jj++){
                    grid.state(ii,endY+jj) = mpBmod->getBoundary(grid.state(ii,endY-1-jj),mDim,t);
                }
            }
            break;
        case INTERNAL :
            if( mDim=='x'){
                for( int jj=startY; jj<endY; jj++){
                    for( int ii=startX; ii<endX; ii++){
                        levelset = grid.levelset(ii,jj);
                        if( levelset > 0){ // PEC
                            if( grid.levelset(ii-1,jj) * levelset < 0 ){ // sign changes
                                for( int kk=0; kk<bound; kk++){
                                    grid.state(ii+kk,jj) = mpBmod->getBoundary(grid.state(ii-1-kk,jj),mDim,t);
                                }
                            }
                            if( mpGrid->levelset(ii+1,jj) * levelset < 0){ // sign changes
                                for( int kk=0; kk<bound; kk++){
                                    grid.state(ii-kk,jj) = mpBmod->getBoundary(grid.state(ii+1+kk,jj),mDim,t);
                                }
                            }
                        }
                    }
                }
            }
            if( mDim=='y'){
                for( int jj=startY; jj<endY; jj++){
                    for( int ii=startX; ii<endX; ii++){
                        levelset = grid.levelset(ii,jj);
                        if( levelset > 0){ // PEC
                            if( grid.levelset(ii,jj-1) * levelset < 0 ){ // sign changes
                                for( int kk=0; kk<bound; kk++){
                                    grid.state(ii,jj+kk) = mpBmod->getBoundary(grid.state(ii,jj-1-kk),mDim,t);
                                }
                            }
                            if( mpGrid->levelset(ii,jj+1) * levelset < 0){ // sign changes
                                for( int kk=0; kk<bound; kk++){
                                    grid.state(ii,jj-kk) = mpBmod->getBoundary(grid.state(ii,jj+1+kk),mDim,t);
                                }
                            }
                        }
                    }
                }
            }
            break;
        default:
            std::cerr << "ERROR: OperatorBoundary bad region: " << mREGION << std::endl;
            exit(EXIT_FAILURE);
    }
}

}// Namespace closure
#endif /* OPERATORBOUNDARY_HPP */
