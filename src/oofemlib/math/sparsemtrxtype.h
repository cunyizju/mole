// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef sparsematrixtype_h
#define sparsematrixtype_h

namespace oofem {
/**
 * Enumerative type used to identify the sparse matrix type.
 */
enum SparseMtrxType {
    SMT_Skyline,       ///< Symmetric skyline.
    SMT_SkylineU,      ///< Unsymmetric skyline.
    SMT_CompCol,       ///< Compressed column.
    SMT_DynCompCol,    ///< Dynamically growing compressed column.
    SMT_SymCompCol,    ///< Symmetric compressed column.
    SMT_DynCompRow,    ///< Dynamically growing compressed row.
    SMT_SpoolesMtrx,   ///< Spooles sparse mtrx representation.
    SMT_PetscMtrx,     ///< PETSc library mtrx representation.
    SMT_DSS_sym_LDL,   ///< Richard Vondracek's sparse direct solver.
    SMT_DSS_sym_LL,    ///< Richard Vondracek's sparse direct solver.
    SMT_DSS_unsym_LU   ///< Richard Vondracek's sparse direct solver.
};
} // end namespace oofem
#endif // sparsematrixtype_h
