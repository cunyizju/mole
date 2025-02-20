// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef bcvaltype_h
#define bcvaltype_h

namespace oofem {
/// Type determining the type of general boundary condition.
enum bcValType {
    UnknownBVT,
    TemperatureBVT,
    ForceLoadBVT,
    PressureBVT,
    HumidityBVT,
    VelocityBVT,
    DisplacementBVT,
    EigenstrainBVT,
    ReinforceBVT,
};
} // end namespace oofem
#endif // bcvaltype_h
