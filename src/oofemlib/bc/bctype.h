// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef bctype_h
#define bctype_h

namespace oofem {
/// Type representing the type of bc.
enum bcType {
    UnknownBT,      ///< Unknown.
    DirichletBT,    ///< Prescribed value.
    TransmissionBC, ///< Neumann type (prescribed flux).
    ConvectionBC,   ///< Newton type - transfer coefficient
    SlipWithFriction,
    PenetrationWithResistance,
    OutFlowBC,
    RadiationBC     ///< Stefan-Boltzmann law.
};
} // end namespace oofem
#endif // bctype_h
