// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef geneigvalsolvertype_h
#define geneigvalsolvertype_h

namespace oofem {
/**
 * Types of general eigenvalue solvers.
 */
enum GenEigvalSolverType {
    GES_SubspaceIt,
    GES_InverseIt,
    GES_SLEPc
};
} // end namespace oofem
#endif // geneigvalsolvertype_h
