// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef loadresponsemode_h
#define loadresponsemode_h

namespace oofem {
/**
 * Type representing the required character of load vector.
 */
enum LoadResponseMode {
    TotalLoad,
    IncrementOfLoad,
};
} // end namespace oofem
#endif // loadresponsemode_h
