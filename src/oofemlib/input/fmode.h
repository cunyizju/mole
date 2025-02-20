// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef fmode_h
#define fmode_h

namespace oofem {
/**
 * Type representing the type of formulation (total or updated) of non-linear computation.
 */
enum fMode {
    UNKNOWN = 0, ///< Unknown.
    TL = 1,      ///< Total Lagrange.
    AL = 2,      ///< Updated Lagrange.
};
} // end namespace oofem
#endif // fmode_h
