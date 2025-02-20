// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef contextoutputmode_h
#define contextoutputmode_h

namespace oofem {
enum ContextOutputMode {
    COM_NoContext,     ///< No context.
    COM_Always,        ///< Enable for post-processing.
    COM_Required,      ///< If required (for backtracking computation).
    COM_UserDefined,   ///< Input attribute of domain (each n-th step).
};
} // end namespace oofem
#endif // contextoutputmode_h
