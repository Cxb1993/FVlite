// OperatorBoundaryManager.hpp
//
// Coordinates boundary update operators.

#ifndef OPERATORBOUNDARYMANAGER_HPP
#define OPERATORBOUNDARYMANAGER_HPP

#include <list>
#include <libconfig.h++>

#include "OperatorBoundary.hpp"
#include "Utils/Composite.hpp"
#include "Boundaries/BoundaryModules.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class OperatorBoundaryManager : public CompositeOperator {
public:
    OperatorBoundaryManager(){}
    virtual void init( Setting& cfg);
};

REGISTER(Operator,BoundaryManager)

void OperatorBoundaryManager::init( Setting& cfg){
    char dim;
    try{
        dim = cfg.lookup("dim").c_str()[0];
    } catch ( const std::exception& e){
        dim = 'x';
    }
    int nMods = cfg.getLength();
    Operator* pOperator;
    string modType;
    for( int count=0; count<nMods; count++){
        Setting& modCfg = cfg[count];
        try{
            modType = modCfg.lookup("type").c_str();
        } catch( const std::exception& e){
            // Skip if other information is found
            continue;
        }
        pOperator = new OperatorBoundary;
        OperatorBoundary* p_op_bound = static_cast<OperatorBoundary*>(pOperator);
        p_op_bound->init( dim, modCfg);
        add_element( p_op_bound);
    }
    return;
}

}// Namespace closure
#endif /* OPERATORBOUNDARYMANAGER_HPP */
