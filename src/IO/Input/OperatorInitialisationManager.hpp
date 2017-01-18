// InitialisationManager.hpp
//
// Coordinates initialisation modules.

#ifndef INITIALISATIONMANAGER_HPP
#define INITIALISATIONMANAGER_HPP

#include <list>
#include <libconfig.h++>

#include "Utils/Composite.hpp"
#include "OperatorInitialisation.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class OperatorInitialisationManager : public CompositeOperator<OperatorInitialisation>{
public:
    OperatorInitialisationManager(){}
    virtual void init( Setting& cfg);
};

REGISTER( Operator, InitialisationManager)

void OperatorInitialisationManager::init( Setting& cfg){
    int nMods = cfg.getLength();
    OperatorInitialisation* pOperator;
    string modType;
    for( int count=0; count<nMods; count++){
        Setting& modCfg = cfg[count];
        try{
            modType = modCfg.lookup("type").c_str();
        } catch( const std::exception& e){
            // skip if other information found
            continue;
        }
        pOperator = new OperatorInitialisation;
        pOperator->init( modCfg);
        add_element( pOperator);
    }
}

}// Namespace closure
#endif /* OPERATORINITIALISATIONMANAGER_HPP */
