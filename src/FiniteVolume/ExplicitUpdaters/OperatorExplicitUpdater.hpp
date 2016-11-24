// OperatorExplicitUpdater.hpp
//
// Operator for finite volume explicit update schemes.
// Traverses grid and updates state vectors using pre-computing
// flux vectors. Note that OperatorFluxSolver performs the task
// of generating fluxes.
//
// This is the abstract version, as variations on finite volume
// schemes may require alternative update formulae. See Operator-
// ExplicitUpdaterEuler for the 'standard' update formula.

#ifndef OPERATOREXPLICITUPDATER_HPP
#define OPERATOREXPLICITUPDATER_HPP

#include <libconfig.h++>

#include "Grid/Grid.hpp"
#include "Timer/Timer.hpp"

using std::string;
using libconfig::Setting;

namespace FVlite{

class OperatorExplicitUpdater : public Operator {
protected:
    char m_dim;
    double m_dt_ratio;
public:
    OperatorExplicitUpdater(){}
    virtual ~OperatorExplicitUpdater(){}

    virtual void init( Setting& cfg);
    virtual void exec( Grid& grid, Timer& timer) = 0;
};

void OperatorExplicitUpdater::init( Setting& cfg){
    m_dim = cfg.lookup("dim").c_str()[0];
    try{
        m_dt_ratio = cfg.lookup("dt_ratio");
    } catch ( const std::exception& e ){
        m_dt_ratio = 1.0;
    }
}

} // namespace closure
#endif // OPERATOREXPLICITUPDATER_hpp
