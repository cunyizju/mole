// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef elementextension_h
#define elementextension_h

namespace oofem {
/**
 * Type representing element extension.
 */
enum ElementExtension {
    // id indicating element class extensions for run time testing
    Element_SurfaceLoadSupport, ///< Element extension for surface loads.
    Element_EdgeLoadSupport,    ///< Element extension for edge loads.
};
} // end namespace oofem
#endif // elementextension_h
