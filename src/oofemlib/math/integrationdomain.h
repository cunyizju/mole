// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef integrationdomain_h
#define integrationdomain_h

namespace oofem {
/**
 * Used by integrator class to supply
 * integration points for proper domain to be
 * integrated (Area,Volume and its shape)
 */
enum integrationDomain {
    _UnknownIntegrationDomain,
    _Point,
    _Line,
    _Triangle,
    _Square,
    _Cube,
    _Tetrahedra,
    _Wedge,
    _Embedded2dLine,
    _3dDegShell,
};
} // end namespace oofem
#endif // integrationdomain_h
