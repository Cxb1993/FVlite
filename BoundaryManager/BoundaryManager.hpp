// BoundaryManager.hpp
//
// Coordinates boundary update modules.

#ifndef BOUNDARYMANAGER_HPP
#define BOUNDARYMANAGER_HPP

#include <list>
#include <libconfig.h++>

#include "BoundaryModules/BoundaryModules.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class BoundaryManager{

protected:

    std::list<BoundaryModule*> Modules;
    std::list<BoundaryModule*>::iterator iter;

public:

    BoundaryManager(){}
    ~BoundaryManager();

    void init( Grid* pGrid, Setting& cfg);

    void add_module( BoundaryModule* Module);
    void sort_modules();
    void exec( char dim);

    // Function used to sort boundary modules
    static bool compare_modules( BoundaryModule* Mod1, BoundaryModule* Mod2);

};

BoundaryManager::~BoundaryManager(){
    for( iter=Modules.begin(); iter!=Modules.end(); iter++){
        delete *iter;
    }
}

void BoundaryManager::init( Grid* pGrid, Setting& cfg){
    int nMods = cfg.getLength();
    BoundaryModule* pBmod;
    string modType;
    for( int count=0; count<nMods; count++){
        Setting& modCfg = cfg[count];
        modType = modCfg.lookup("type").c_str();
        pBmod = BoundaryModuleFactory.create( modType);
        pBmod->init( pGrid, modCfg);
        add_module( pBmod);
    }
    sort_modules();
    return;
}

void BoundaryManager::add_module( BoundaryModule* pModule){
    Modules.push_back(pModule);
    return;
}

void BoundaryManager::sort_modules(){
    Modules.sort(BoundaryManager::compare_modules);
    return;
}

void BoundaryManager::exec( char dim){
    for( iter=Modules.begin(); iter!=Modules.end(); iter++){
        (*iter)->exec(dim);
    }
    return;
}

bool BoundaryManager::compare_modules( BoundaryModule* Mod1, BoundaryModule* Mod2){
    return (Mod1->rank() < Mod2->rank());
}

}// Namespace closure
#endif /* BOUNDARYMANAGER_HPP */
