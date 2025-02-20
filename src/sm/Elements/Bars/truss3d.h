// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef truss3d_h
#define truss3d_h

#include "sm/Elements/nlstructuralelement.h"
#include "sm/ErrorEstimators/directerrorindicatorrc.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"

#define _IFT_Truss3d_Name "truss3d"

namespace oofem {
class FEI3dLineLin;

/**
 * This class implements a two-node truss bar element for three-dimensional
 * analysis.
 */
class Truss3d : public NLStructuralElement,
    public ZZNodalRecoveryModelInterface,
    public NodalAveragingRecoveryModelInterface
{
protected:
    static FEI3dLineLin interp;

public:
    Truss3d(int n, Domain *d);
    virtual ~Truss3d() { }

    FEInterpolation *giveInterpolation() const override;

    double computeLength() override;

    void computeInitialStressMatrix(FloatMatrix &answer, TimeStep *tStep) override;
    void computeLumpedMassMatrix(FloatMatrix &answer, TimeStep *tStep) override;
    void computeMassMatrix(FloatMatrix &answer, TimeStep *tStep) override
    { this->computeLumpedMassMatrix(answer, tStep); }
    int giveLocalCoordinateSystem(FloatMatrix &answer) override;

    int computeNumberOfDofs() override { return 6; }
    void giveDofManDofIDMask(int inode, IntArray &) const override;


    // characteristic length (for crack band approach)
    double giveCharacteristicLength(const FloatArray &normalToCrackPlane) override
    { return this->computeLength(); }

    double computeVolumeAround(GaussPoint *gp) override;

    int testElementExtension(ElementExtension ext) override { return ( ext == Element_EdgeLoadSupport ); }

    Interface *giveInterface(InterfaceType it) override;

    void NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node, InternalStateType type, TimeStep *tStep) override;

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_Truss3d_Name; }
    const char *giveClassName() const override { return "Truss3d"; }
    void initializeFrom(InputRecord &ir) override;
    MaterialMode giveMaterialMode() override { return _1dMat; }
    void computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep) override;
    void computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override;
    void computeConstitutiveMatrix_dPdF_At(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override;

protected:
    // edge load support
    void giveEdgeDofMapping(IntArray &answer, int iEdge) const override;
    double computeEdgeVolumeAround(GaussPoint *gp, int) override;
    int computeLoadLEToLRotationMatrix(FloatMatrix &answer, int, GaussPoint *gp) override;
    void computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int = 1, int = ALL_STRAINS) override;
    void computeBHmatrixAt(GaussPoint *gp, FloatMatrix &answer) override;
    void computeNmatrixAt(const FloatArray &iLocCoord, FloatMatrix &answer) override;
    void computeGaussPoints() override;
};
} // end namespace oofem
#endif // truss3d_h
