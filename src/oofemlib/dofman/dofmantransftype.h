// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef dofmantransftype_h
#define dofmantransftype_h

namespace oofem {
/**
 * Enumerative type, used to specify type of transformation required from dofManager (node).
 * Then global vector @f$ f_g @f$ can be obtained by following operation
 * @f$ f_g = T\cdot f_n@f$, where @f$T@f$ is transformation matrix and @f$f_n@f$ is vector expressed in
 * nodal coordinate system.
 */
enum DofManTransfType {
    _toGlobalCS, ///< Transformation from global c.s in node to node-dependent coordinate system.
    _toNodalCS, ///< Transformation from node-dependent coordinate system to global coordinate system in node.
};
} // end namespace oofem
#endif // dofmantransftype_h
