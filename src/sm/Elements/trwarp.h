// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef tr_warp_h
#define tr_warp_h

#include "structuralelement.h"
#include "mesher/spatiallocalizer.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "fei/fei2dtrlin.h"

#define _IFT_Tr_Warp_Name "trwarp"


namespace oofem {
/**
 * Triangle (2d) element with linear approximation for free warping analysis.
 */
class Tr_Warp : public StructuralElement, public SpatialLocalizerInterface, public ZZNodalRecoveryModelInterface
{
protected:
    static FEI2dTrLin interp;

public:
    Tr_Warp(int n, Domain *d);
    virtual ~Tr_Warp();
    void computeFirstMomentOfArea(FloatArray &answer);
    double computeVolumeAround(GaussPoint *gp) override;
    void giveEdgeDofMapping(IntArray &answer, int iEdge) const override;
    void computeEdgeLoadVectorAt(FloatArray &answer, Load *load, TimeStep *tStep, ValueModeType mode);
    void giveCharacteristicVector(FloatArray &answer, CharType mtrx, ValueModeType mode, TimeStep *tStep) override;
    // definition
    const char *giveInputRecordName() const override { return _IFT_Tr_Warp_Name; }
    const char *giveClassName() const override { return "Tr_WarpElement"; }

    int computeNumberOfDofs() override { return 4; }
    void initializeFrom(InputRecord &ir) override;
    MaterialMode giveMaterialMode() override { return _Warping; }
    double giveThicknessAt(const FloatArray &gcoords);
    void computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep) override;
    void computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override;
    void computeNmatrixAt(const FloatArray &iLocCoord, FloatMatrix &answer) override;
    void computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int li, int ui) override;
    void giveDofManDofIDMask(int inode, IntArray &answer) const override;

    Interface *giveInterface(InterfaceType t) override;

    void giveInternalDofManDofIDMask(int inode, IntArray &answer) const override;
    DofManager *giveInternalDofManager(int i) const override;

    int giveNumberOfInternalDofManagers() const override { return 1; }

    void ZZNodalRecoveryMI_computeNNMatrix(FloatArray &answer, InternalStateType type) override;
    bool ZZNodalRecoveryMI_computeNValProduct(FloatMatrix &answer, InternalStateType type, TimeStep *tStep) override;
    int SpatialLocalizerI_containsPoint(const FloatArray &coords) override;

    FEInterpolation *giveInterpolation() const override { return & this->interp; }
    Element_Geometry_Type giveGeometryType() const override { return EGT_triangle_1; }

protected:
    void computeGaussPoints() override;
    double computeEdgeVolumeAround(GaussPoint *gp, int iEdge) override;
    void transformCoordinates(FloatArray &answer, FloatArray &c, const int CGnumber);
    void postInitialize() override;
};
} // end namespace oofem
#endif // tr_warp_h
