// BoundaryManager.hpp
//
// Coordinates boundary update modules.

#ifndef BOUNDARYMANAGER_HPP
#define BOUNDARYMANAGER_HPP

#include <list>
#include <libconfig.h++>

#include "Utils/GridManager.hpp"
#include "BoundaryModules.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class BoundaryManager : public GridManager<BoundaryModule>{
public:

    BoundaryManager(){}
    virtual void init( Grid* pGrid, Setting& cfg);
};

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


}// Namespace closure
#endif /* BOUNDARYMANAGER_HPP */
