// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef numericalcmpn_h
#define numericalcmpn_h

namespace oofem {
/**
 * Type representing numerical component. The components of characteristic equations are mapped
 * to their corresponding numerical counterparts using these common component types.
 * All numerical methods solving the same problem have to use the same and compulsory
 * NumericalCmpn values. This allows to use generally any numerical method instance (even added in future)
 * without changing any code.
 */
enum NumericalCmpn {
    InternalRhs,
    NonLinearLhs,
    ExternalRhs,
};
} // end namespace oofem
#endif // numericalcmpn_h
