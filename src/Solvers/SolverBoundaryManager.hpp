// SolverBoundaryManager.hpp
//
// Coordinates boundary update modules.

#ifndef SOLVERBOUNDARYMANAGER_HPP
#define SOLVERBOUNDARYMANAGER_HPP

#include <list>
#include <libconfig.h++>

#include "Utils/GridManager.hpp"
#include "Boundaries/BoundaryModules.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class SolverBoundaryManager : public SolverDecorator, public GridManager<BoundaryModule>{
protected:
    char m_dim;
public:
    SolverBoundaryManager(){}
    virtual void init( Grid* pGrid, Setting& cfg){(void)pGrid;(void)cfg;} // Don't call this, due to be deprecated
    virtual void init( Grid* pGrid, Timer* pTimer, Setting& cfg, Solver* p_wrapped);
    virtual void exec();
};

REGISTER(Solver,BoundaryManager)

void SolverBoundaryManager::init( Grid* pGrid, Timer* pTimer, Setting& cfg, Solver* p_wrapped){
    SolverDecorator::init(pGrid,pTimer,cfg,p_wrapped);
    try{
        m_dim = cfg.lookup("dim").c_str()[0];
    } catch ( const std::exception& e){
        m_dim = 'x';
    }
    int nMods = cfg.getLength();
    BoundaryModule* pBmod;
    string modType;
    for( int count=0; count<nMods; count++){
        Setting& modCfg = cfg[count];
        try{
            modType = modCfg.lookup("type").c_str();
        } catch( const std::exception& e){
            // Catch other info, like dim and solver type
            continue;
        }
        pBmod = BoundaryModuleFactory.create( modType);
        pBmod->init( pGrid, modCfg);
        add_module( pBmod);
    }
    sort_modules();
    return;
}

void SolverBoundaryManager::exec(){
    SolverDecorator::exec();
    for( iter=Modules.begin(); iter!=Modules.end(); iter++){
        (*iter)->exec(m_dim,mpTimer->t());
    }
    return;
}

}// Namespace closure
#endif /* SOLVERBOUNDARYMANAGER_HPP */
