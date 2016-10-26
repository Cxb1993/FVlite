// GridManager.hpp
//
// Coordinates 'modules' which act on the grid.
// Used by both boundary management and initialisation systems.
// Templated to avoid repeated code.
// Useless in this state -- requires an init function.

#ifndef GRIDMANAGER_HPP
#define GRIDMANAGER_HPP

#include <list>
#include <libconfig.h++>

#include "Grid/Grid.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

template<class m>
class GridManager{

protected:

    typename std::list<m*> Modules;
    typename std::list<m*>::iterator iter;

public:

    GridManager(){}
    virtual ~GridManager();

    virtual void init( Grid* pGrid, Setting& cfg) = 0;

    void add_module( m* Module);
    void sort_modules();

    // Function used to sort modules by 'rank'
    static bool compare_modules( m* Mod1, m* Mod2);

};

template<class m>
GridManager<m>::~GridManager(){
    for( iter=Modules.begin(); iter!=Modules.end(); iter++){
        delete *iter;
    }
}

template<class m>
void GridManager<m>::add_module( m* pModule){
    Modules.push_back(pModule);
    return;
}

template<class m>
void GridManager<m>::sort_modules(){
    Modules.sort(GridManager::compare_modules);
    return;
}

template<class m>
bool GridManager<m>::compare_modules( m* Mod1, m* Mod2){
    return (Mod1->rank() < Mod2->rank());
}

}// Namespace closure
#endif /* GRIDMANAGER_HPP */
