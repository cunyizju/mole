// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef crosssectextension_h
#define crosssectextension_h

namespace oofem {
/// Type representing cross section extension for run time testing.
enum CrossSectExtension {
    CS_StructuralCapability, ///< Structural capability.
    CS_StructuralInterfaceCapability, ///< Structural interface capability.
    CS_HeatCapability, ///< Heat capability.
    CS_LatticeStructuralCapability, ///< Structural lattice capability.
};
} // end namespace oofem
#endif // crosssectextension_h
