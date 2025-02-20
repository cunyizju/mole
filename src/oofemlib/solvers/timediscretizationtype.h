// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef timediscretizationtype_h
#define timediscretizationtype_h

namespace oofem {
/// Time discretization used by transient solvers.
enum TimeDiscretizationType {
    TD_Unspecified         = -1,     ///< Unspecified
    TD_ThreePointBackward  =  0,     ///< Three-point Backward Euler method
    TD_TwoPointBackward    =  1,     ///< Two-point Backward Euler method
    TD_Newmark             =  2,     ///< Newmark-beta method
    TD_Wilson              =  3,     ///< Wilson-theta method
    TD_Explicit            =  4,     ///< Central difference
};
} // end namespace oofem
#endif // timediscretizationtype_h
