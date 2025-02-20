// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef stressstrainprincmode_h
#define stressstrainprincmode_h

namespace oofem {
/**
 * We have only one algorithm for computing eigenvalues and vectors
 * in order to be able to distinguish between some different modes we define
 * this new type.
 */
enum stressStrainPrincMode {
    principal_strain, ///< For computing principal strains from engineering strains.
    principal_stress, ///< For computing principal stresses.
    principal_deviatoricstress, ///< For computing principal stresses from deviatoric stress.
};
} // end namespace oofem
#endif // stressstrainprincmode_h
