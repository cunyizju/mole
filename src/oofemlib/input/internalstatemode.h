// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef internalstatemode_h
#define internalstatemode_h

namespace oofem {
/// Determines the mode of internal variable.
enum InternalStateMode {
    ISM_local,
    ISM_recovered
};
} // end namespace oofem
#endif // internalstatemode_h
