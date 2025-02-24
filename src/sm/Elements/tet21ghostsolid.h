// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef TET21GHOSTSOLID_H
#define TET21GHOSTSOLID_H

#include "sm/Elements/nlstructuralelement.h"
#include "math/floatmatrix.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"
#include "mesher/spatiallocalizer.h"
#include "input/eleminterpmapperinterface.h"

#define _IFT_tet21ghostsolid_Name "tet21ghostsolid"

namespace oofem {
class FEI3dTetQuad;
class FEI3dTetLin;

class tet21ghostsolid : public NLStructuralElement,
    public NodalAveragingRecoveryModelInterface,
    public SpatialLocalizerInterface,
    public EIPrimaryUnknownMapperInterface
{
private:
    FloatMatrix Dghost;
    bool computeItransform;
    FloatMatrix Itransform;
    static IntArray velocitydofsonside;
    static IntArray displacementdofsonside;

    void giveUnknownData(FloatArray &u_prev, FloatArray &u, FloatArray &inc, TimeStep *tStep);

public:
    tet21ghostsolid(int n, Domain *d);

    FEInterpolation *giveInterpolation() const override;
    FEInterpolation *giveInterpolation(DofIDItem id) const override;

    void giveDofManDofIDMask(int inode, IntArray &answer) const override;
    const char *giveInputRecordName() const override { return _IFT_tet21ghostsolid_Name; }
    int computeNumberOfDofs() override { return 70; }
    void computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep) override;
    void computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override;
    void computeConstitutiveMatrix_dPdF_At(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override;
    MaterialMode giveMaterialMode() override { return _3dMat; }
    void computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep) override;
    virtual void computeNumericStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep);
    void giveInternalForcesVector(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord = 0) override;
    void giveInternalForcesVectorGivenSolution(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord, FloatArray &SolutionVector);
    void computeLoadVector(FloatArray &answer, BodyLoad *load, CharType type, ValueModeType mode, TimeStep *tStep) override;
    void computeBoundarySurfaceLoadVector(FloatArray &answer, BoundaryLoad *load, int boundary, CharType type, ValueModeType mode, TimeStep *tStep, bool global = true) override;

    void computeDeformationGradientVectorFromDispl(FloatArray &answer, GaussPoint *gp, TimeStep *tStep, FloatArray &u);
    void computeDeformationGradientVector(FloatArray &answer, GaussPoint *gp, TimeStep *tStep) override;
    void computeDeformationGradientVectorAt(FloatArray &answer, FloatArray lcoord, TimeStep *tStep);

    int giveIPValue(FloatArray &answer, GaussPoint *gp, InternalStateType type, TimeStep *tStep) override;
    double computeVolumeAround(GaussPoint *gp) override;
    bool giveRowTransformationMatrix(TimeStep *tStep);
    const char *giveClassName() const override { return "tet21ghostsolid"; }

    void computeNumericStiffnessMatrixDebug(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep);
    void giveInternalForcesVectorGivenSolutionDebug(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord, FloatArray &SolutionVector, bool ExtraLogging);

    Interface *giveInterface(InterfaceType it) override;

    // Element interpolation interface:
    void EIPrimaryUnknownMI_computePrimaryUnknownVectorAtLocal(ValueModeType u, TimeStep *tStep, const FloatArray &coords, FloatArray &answer) override;

    // Nodal averaging interface:
    void NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node, InternalStateType type, TimeStep *tStep) override;

protected:
    static FEI3dTetQuad interpolation;
    static FEI3dTetLin interpolation_lin;

    void computeBmatrixAt(GaussPoint *, FloatMatrix &, int = 1, int = ALL_STRAINS) override;
    void computeBHmatrixAt(GaussPoint *, FloatMatrix &) override;
    void computeGaussPoints() override;

    /// Ordering of momentum balance equations in element. Used to assemble the element stiffness
    static IntArray momentum_ordering;
    /// Ordering of conservation equations in element. Used to assemble the element stiffness
    static IntArray conservation_ordering;
    /// Ordering of ghost displacements equations
    static IntArray ghostdisplacement_ordering;
};
}
#endif // TET21GHOSTSOLID_H
