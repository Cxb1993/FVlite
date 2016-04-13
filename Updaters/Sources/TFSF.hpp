// TFSF.hpp
//
// Implementation of the 'Total Field, Scattered Field' formulation.
// Applies a source wave to the system. Uses the linearity of Maxwell's equations to state:
// E_total = E_incident + E_scattered
// Within a central region, both the incident and scattered fields are modelled. Outside, only
// the scattered field is present. The boundary between these regions must be monitored to
// include/exclude the incident field.


// TODO
