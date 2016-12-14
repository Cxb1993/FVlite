#ifdef MAXWELL
#include "Materials/MaterialElectromagnetic.hpp"
namespace FVlite{
typedef MaterialElectromagnetic Material;
}
#endif

#ifdef EULER
#include "Materials/MaterialIdealGas.hpp"
namespace FVlite{
typedef MaterialIdealGas Material;
}
#endif
