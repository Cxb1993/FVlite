// OperatorInitialiseBase.hpp
//
// Abstract Initialise operator.
// Writes to grid from user-defined settings.

#ifndef OPERATORINITIALISEBASE_HPP
#define OPERATORINITIALISEBASE_HPP

#include <libconfig.h++>

#include "Grid/Grid.hpp"
#include "LevelSetMethods/LevelSetMethods.hpp"

using libconfig::Setting;

namespace FVlite{

class OperatorInitialiseBase : public Operator {

protected:

    bool is_inverted;

    SignedDistance* mp_signdist;

public:

    OperatorInitialiseBase(){}
    virtual ~OperatorInitialiseBase(){}

    virtual void init( Setting& cfg) = 0;
    virtual void exec( Grid& grid, Timer& timer) = 0;
    double get_distance( const Vector3& x) const { return mp_signdist->get_distance(x); }
};


}
#endif
