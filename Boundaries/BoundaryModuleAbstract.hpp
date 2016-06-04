// BoundaryModuleAbstract.hpp
//
// Abstract boundary module.

#ifndef BOUNDARYMODULEABSTRACT_HPP
#define BOUNDARYMODULEABSTRACT_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <libconfig.h++>

#include "Grid/Grid.hpp"
#include "Utils/ObjectFactory.hpp"

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

class BoundaryModule{

protected:

    Grid* pGrid;

    BOUNDARY_REGION mREGION;
    int mRank;

public:

    BoundaryModule(){}
    BoundaryModule( const BoundaryModule& other);
    virtual ~BoundaryModule(){}

    void init( Grid* pGrid, Setting& cfg);

    void exec( char dim);
    virtual StateVector getBoundary( const StateVector& Reference, const char dim) = 0;
    int rank(){ return mRank;}

};

// Setting up factory

ObjectFactory<BoundaryModule> BoundaryModuleFactory;

// Function definitions

void BoundaryModule::init( Grid* pGrid, Setting& cfg){
    (*this).pGrid = pGrid;
    string region = cfg.lookup("region");
    if( region == "leftwall")  mREGION = LEFTWALL;
    if( region == "rightwall") mREGION = RIGHTWALL;
    if( region == "upperwall") mREGION = UPPERWALL;
    if( region == "lowerwall") mREGION = LOWERWALL;
    if( region == "internal")  mREGION = INTERNAL;
    mRank = (mREGION==INTERNAL) ? 1 : 2;
    return;
}

BoundaryModule::BoundaryModule( const BoundaryModule& other) 
    : pGrid(other.pGrid), mREGION(other.mREGION), mRank(other.mRank)
{}

void BoundaryModule::exec( char dim){

    int bound  = pGrid->bound();
    int startX = pGrid->startX();
    int startY = pGrid->startY();
    int endX   = pGrid->endX();
    int endY   = pGrid->endY();
    double levelset;

    switch( mREGION ){
        case LEFTWALL :
            if( dim != 'x') break;
            for( int ii=0; ii<bound; ii++){
                for( int jj=startY; jj<endY; jj++){
                    pGrid->state(startX-1-ii,jj) = getBoundary(pGrid->state(startX+ii,jj),dim);
                }
            }
            break;
        case RIGHTWALL :
            if( dim != 'x') break;
            for( int ii=0; ii<bound; ii++){
                for( int jj=startY; jj<endY; jj++){
                    pGrid->state(endX+ii,jj) = getBoundary(pGrid->state(endX-1-ii,jj),dim);
                }
            }
            break;
        case LOWERWALL :
            if( dim != 'y') break;
            for( int ii=startX; ii<endX; ii++){
                for( int jj=0; jj<bound; jj++){
                    pGrid->state(ii,startY-1-jj) = getBoundary(pGrid->state(ii,startY+jj),dim);
                }
            }
            break;
        case UPPERWALL :
            if( dim != 'y') break;
            for( int ii=startX; ii<endX; ii++){
                for( int jj=0; jj<bound; jj++){
                    pGrid->state(ii,endY+jj) = getBoundary(pGrid->state(ii,endY-1-jj),dim);
                }
            }
            break;
        case INTERNAL :
            if( dim=='x'){
                for( int jj=startY; jj<endY; jj++){
                    for( int ii=startX; ii<endX; ii++){
                        levelset = pGrid->levelset(ii,jj);
                        if( levelset > 0){ // PEC
                            if( pGrid->levelset(ii-1,jj) * levelset < 0 ){ // sign changes
                                for( int kk=0; kk<bound; kk++){
                                    pGrid->state(ii+kk,jj) = getBoundary(pGrid->state(ii-1-kk,jj),dim);
                                }
                            }
                            if( pGrid->levelset(ii+1,jj) * levelset < 0){ // sign changes
                                for( int kk=0; kk<bound; kk++){
                                    pGrid->state(ii-kk,jj) = getBoundary(pGrid->state(ii+1+kk,jj),dim);
                                }
                            }
                        }
                    }
                }
            }
            if( dim=='y'){
                for( int jj=startY; jj<endY; jj++){
                    for( int ii=startX; ii<endX; ii++){
                        levelset = pGrid->levelset(ii,jj);
                        if( levelset > 0){ // PEC
                            if( pGrid->levelset(ii,jj-1) * levelset < 0 ){ // sign changes
                                for( int kk=0; kk<bound; kk++){
                                    pGrid->state(ii,jj+kk) = getBoundary(pGrid->state(ii,jj-1-kk),dim);
                                }
                            }
                            if( pGrid->levelset(ii,jj+1) * levelset < 0){ // sign changes
                                for( int kk=0; kk<bound; kk++){
                                    pGrid->state(ii,jj-kk) = getBoundary(pGrid->state(ii,jj+1+kk),dim);
                                }
                            }
                        }
                    }
                }
            }
            break;
        default:
            std::cerr << "ERROR: BoundaryModule bad region: " << mREGION << std::endl;
            exit(EXIT_FAILURE);
    }

    return;
}

}// Namespace closure
#endif /* BOUNDARYMODULEABSTRACT_HPP */
