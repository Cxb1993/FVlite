// InitialisationManager.hpp
//
// Coordinates initialisation modules.

#ifndef INITIALISATIONMANAGER_HPP
#define INITIALISATIONMANAGER_HPP

#include <list>
#include <libconfig.h++>

#include "Utils/GridManager.hpp"
#include "InitialisationModules.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class InitialisationManager : public GridManager<InitialisationModule>{
public:

    InitialisationManager(){}
    virtual void init( Grid* pGrid, Setting& cfg);
};

void InitialisationManager::init( Grid* pGrid, Setting& cfg){
    int nMods = cfg.getLength();
    InitialisationModule* pImod;
    string modType;
    for( int count=0; count<nMods; count++){
        Setting& modCfg = cfg[count];
        modType = modCfg.lookup("type").c_str();
        pImod = InitialisationModuleFactory.create( modType);
        pImod->init( pGrid, modCfg);
        add_module( pImod);
    }
    sort_modules();
    return;
}


}// Namespace closure
#endif /* INITIALISATIONMANAGER_HPP */
