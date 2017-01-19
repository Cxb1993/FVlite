// OperatorInitialise.hpp
//
// Composite over Initialise Operators

#ifndef OPERATORINITIALISE_HPP
#define OPERATORINITIALISE_HPP

#include <string>
#include <libconfig.h++>

#include "OperatorInitialiseBase.hpp"
#include "Operators/CompositeOperator.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class OperatorInitialise : public CompositeOperator {
public:
    OperatorInitialise(){}
    virtual void init( Setting& cfg);
    // gets exec from CompositeOperator
};

void OperatorInitialise::init( Setting& cfg){
    int nOps = cfg.getLength();
    Operator* pOperator;
    string opType;
    for( int count=0; count<nOps; count++){
        Setting& thisCfg = cfg[count];
        try{
            opType = thisCfg.lookup("type").c_str();
        } catch( const std::exception& e){
            // skip this if other information found
            continue;
        }
        pOperator = OperatorFactory.create("Initialise"+opType);
        pOperator->init( thisCfg);
        add_element( pOperator);
    }
}

}
#endif
