// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef structuralinterfaceelement_h
#define structuralinterfaceelement_h

#include "sm/CrossSections/structuralinterfacecrosssection.h"
#include "input/element.h"
#include "math/floatmatrix.h"
#include "func/function.h"
#include "material/matresponsemode.h"
#include "input/valuemodetype.h"
#include "math/integrationdomain.h"
#include "dofman/dofmantransftype.h"

namespace oofem {
class TimeStep;
class Node;
class StructuralInterfaceMaterial;
class GaussPoint;
class FloatArray;
class IntArray;
class FEInterpolation;


/**
 * Abstract base class for all structural interface elements. It declares a common interface available
 * to all derived elements. The implementation of these services is partly left to the derived classes,
 * some general services are implemented here (but they can be overload by more efficient
 * element implementations).
 * The general implementation provided here is intended for both linear and nonlinear computations.
 */
class StructuralInterfaceElement : public Element
{
protected:
    /// Initial displacement vector, describes the initial nodal displacements when element has been casted.
    FloatArray initialDisplacements;
    /// Flag indicating if geometrical nonlinearities apply.
    bool nlGeometry = false;

public:
    /**
     * Constructor. Creates structural element with given number, belonging to given domain.
     * @param n Element number.
     * @param d Domain to which new material will belong.
     */
    StructuralInterfaceElement(int n, Domain * d);

    int computeGlobalCoordinates(FloatArray &answer, const FloatArray &lcoords) override;

    void giveCharacteristicMatrix(FloatMatrix &answer, CharType, TimeStep *tStep) override;
    void giveCharacteristicVector(FloatArray &answer, CharType type, ValueModeType mode, TimeStep *tStep) override;

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
    virtual void computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep);

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
    virtual void giveInternalForcesVector(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord = 0);
    virtual void computeTraction(FloatArray &traction, IntegrationPoint *ip, const FloatArray &jump, TimeStep *tStep);
    virtual void computeSpatialJump(FloatArray &answer, GaussPoint *gp, TimeStep *tStep);
    int giveIPValue(FloatArray &answer, GaussPoint *gp, InternalStateType type, TimeStep *tStep) override;

    Interface *giveInterface(InterfaceType) override { return nullptr; }

    //@}

    // Overloaded methods.
    void updateInternalState(TimeStep *tStep) override;
    void updateYourself(TimeStep *tStep) override;
    int checkConsistency() override;
    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;
    const char *giveClassName() const override { return "StructuralInterfaceElement"; }

    StructuralInterfaceCrossSection *giveInterfaceCrossSection();
    virtual double computeAreaAround(GaussPoint *gp) = 0;

    Element_Geometry_Type giveGeometryType() const override { return EGT_unknown; }

    //virtual methods that should be overloaded by the elements
    virtual void giveEngTraction(FloatArray &answer, GaussPoint *gp, const FloatArray &jump, TimeStep *tStep);
    virtual void giveFirstPKTraction(FloatArray &answer, GaussPoint *gp, const FloatArray &jump, const FloatMatrix &F, TimeStep *tStep)
    {
        OOFEM_ERROR("not implemented for the current element");
    }

    virtual void giveStiffnessMatrix_Eng(FloatMatrix &answer, MatResponseMode rMode, IntegrationPoint *ip, TimeStep *tStep);
    virtual void giveStiffnessMatrix_dTdj(FloatMatrix &answer, MatResponseMode rMode, IntegrationPoint *ip, TimeStep *tStep)
    {
        OOFEM_ERROR("not implemented for the current element");
    }

protected:
    /**
     * Computes modified interpolation matrix (N) for the element which multiplied
     * with the unknowns vector (u) produces the spatial jump.
     * @param gp Integration point for which answer is assembled.
     * @param answer Interpolation matrix evaluated at gp.
     */
    virtual void computeNmatrixAt(GaussPoint *gp, FloatMatrix &answer) = 0;

    // transformation matrix from local to global
    virtual void computeTransformationMatrixAt(GaussPoint *gp, FloatMatrix &answer) = 0;

    /**
     * Return desired number of integration points for consistent mass matrix
     * computation, if required.
     * @return Number of integration points for mass matrix.
     * @todo not sure if we should allow these elements to have mass - JB
     */
    virtual int giveNumberOfIPForMassMtrxIntegration() { return 0; }

    virtual int testCrossSectionExtension(CrossSectExtension ext) { return ( ( ext == CS_StructuralInterfaceCapability ) ? 1 : 0 ); }
    int testElementExtension(ElementExtension ext) override { return 0; } ///@ necessary to define?
};
} // end namespace oofem
#endif // structuralinterfaceelement_h
