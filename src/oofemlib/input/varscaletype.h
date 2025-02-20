// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef varscaletype_h
#define varscaletype_h

namespace oofem {
/// Type determining the scale corresponding to particular variable.
enum VarScaleType {
    VST_Length,
    VST_Velocity,
    VST_Time,
    VST_Density,
    VST_Pressure,
    VST_Force,
    VST_Viscosity,
    VST_ReynoldsNumber
};
} // end namespace oofem
#endif // varscaletype_h
