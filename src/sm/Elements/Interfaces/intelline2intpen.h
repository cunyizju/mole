// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef SRC_SM_ELEMENTS_INTERFACES_INTELLINE2INTPEN_H_
#define SRC_SM_ELEMENTS_INTERFACES_INTELLINE2INTPEN_H_

#include "intelline2.h"

#define _IFT_IntElLine2IntPen_Name "intelline2intpen"

namespace oofem {
class FEI2dLineQuad;

/**
 * This class implements a two dimensional interface element
 * with interior penalty formulation.
 * @author Erik Svenning
 */
class IntElLine2IntPen : public IntElLine2
{
public:
    IntElLine2IntPen(int n, Domain * d);

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_IntElLine2IntPen_Name; }
    const char *giveClassName() const override { return "IntElLine2IntPen"; }

    void initializeFrom(InputRecord &ir) override;

    FloatArrayF<2> computeCovarBaseVectorAt(GaussPoint *gp) const override;


    /**
     * Computes the stiffness/tangent matrix of receiver. Default implementation computes element stiffness using
     * @f$ K=\int_{\Gamma} N^{\mathrm{T}} D N \mathrm{d}V @f$ formulae, where @f$ N @f$ is the element geometric matrix such
     * that @f$ j = N a @f$ and @f$ D @f$ is the stiffness matrix of the interface material.
     * Numerical integration procedure uses integrationRulesArray for numerical integration.
     *
     * The geometrical matrix is obtained using computeNmatrixAt service and the constitutive matrix is obtained using
     * computeConstitutiveMatrixAt service.
     *
     * For higher numerical performance, only one half of stiffness matrix is computed and answer is then symmetrized.
     * Therefore, if element matrix will be generally nonsymmetric, one must specialize this method.
     * Finally, the result is transformed into global coordinate system (or nodal coordinate system, if it is defined).
     *
     * @param answer Computed stiffness matrix (symmetric).
     * @param rMode Response mode.
     * @param tStep Time step.
     */
    void computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep) override;


    /**
     * Returns equivalent nodal forces vectors. Useful for nonlinear analysis.
     * Default implementation computes result as @f$ F=\int_v B^{\mathrm{T}} \sigma \mathrm{d}V @f$, where @f$ \sigma @f$ is the
     * real element stress vector obtained using computeStressVector service (if useUpdatedGpRecord=0) or
     * (if useUpdatedGpRecord=1) from integration point status.
     * The geometric matrix is obtained using computeBmatrixAt service.
     * Integration is performed using default integration rule, which should produce always valid results,
     * assuming that strains used for computation of stresses are valid.
     * @param answer Internal nodal forces vector.
     * @param tStep Time step.
     * @param useUpdatedGpRecord If equal to zero, the stresses in integration points are computed (slow but safe), else if
     * nonzero the stresses are taken directly from integration point status (should be derived from StructuralMaterialStatus)
     * (fast, but engineering model must ensure valid status data in each integration point).
     */
    void giveInternalForcesVector(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord = 0) override;

};

} /* namespace oofem */

#endif /* SRC_SM_ELEMENTS_INTERFACES_INTELLINE2INTPEN_H_ */
