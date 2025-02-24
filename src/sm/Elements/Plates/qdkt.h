// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef qdkt_h
#define qdkt_h

#include "sm/Elements/structuralelement.h"
#include "sm/CrossSections/layeredcrosssection.h"
#include "sm/ErrorEstimators/zzerrorestimator.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"
#include "nodalrecovery/sprnodalrecoverymodel.h"

#define _IFT_QDKTPlate_Name "qdktplate"

namespace oofem {
class FEI2dQuadLin;

/**
 * This class implements an quad Discrete Kirchhoff Theory (DKT) element.
 * This element is a plate element suitable for thin plates, as the traswerse shear strain energy is neglected.
 * The element has only 12 DOFs (w-displacement and rotations along coordinate axes) in each node
 * The derivation starts by assuming quadratic variations of rotation field (fi_x, fi_y)
 * The Kirchhoff hypothesis is imposed at vertices and in mid side nodes
 * The cubic variation of transwerse displacement is assumed along the edges, there is no need to define interpolation for w on the element.
 * As w varies cubically along the edges, its derivative along the edge varies quadratically as the normal rotation along the edge. This allows to satisfy the Kirchhoff hypothesis along the edge. The rotation along the edge is assumed to vary linearly.
 * This allows to express midside rotations (along and normal to the each edge) as a linear combination of nodal rotations and displacements.
 *
 * Tasks:
 * - calculating its B,D,N matrices and dV.
 */
class QDKTPlate : public StructuralElement,
    public ZZNodalRecoveryModelInterface,
    public SPRNodalRecoveryModelInterface,
    public LayeredCrossSectionInterface,
    public ZZErrorEstimatorInterface
{
protected:
    /// Element geometry approximation
    static FEI2dQuadLin interp_lin;

public:
    QDKTPlate(int n, Domain *d);
    virtual ~QDKTPlate() { }

    FEInterpolation *giveInterpolation() const override;
    FEInterpolation *giveInterpolation(DofIDItem id) const override;

    MaterialMode giveMaterialMode() override { return _2dPlate; }
    int testElementExtension(ElementExtension ext) override { return ( ( ( ext == Element_EdgeLoadSupport ) || ( ext == Element_SurfaceLoadSupport ) ) ? 1 : 0 ); }

protected:
    void computeGaussPoints() override;
    void computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int = 1, int = ALL_STRAINS) override;
    void computeNmatrixAt(const FloatArray &iLocCoord, FloatMatrix &answer) override;

    void computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep) override;
    void computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override;


    virtual void giveNodeCoordinates(double &x1, double &x2, double &x3, double &x4,
                                     double &y1, double &y2, double &y3, double &y4,
                                     double &z1, double &z2, double &z3, double &z4);

    void computeBodyLoadVectorAt(FloatArray &answer, Load *forLoad, TimeStep *tStep, ValueModeType mode) override;

    /**
     * @name Surface load support
     */
    //@{
    void computeSurfaceNMatrix(FloatMatrix &answer, int boundaryID, const FloatArray &lcoords) override;
    void computeSurfaceNMatrixAt(FloatMatrix &answer, int iSurf, GaussPoint *gp);
    void giveSurfaceDofMapping(IntArray &answer, int iSurf) const override;
    double computeSurfaceVolumeAround(GaussPoint *gp, int iSurf) override;
    int computeLoadLSToLRotationMatrix(FloatMatrix &answer, int iSurf, GaussPoint *gp) override;
    //@}
    /**
     * @name Edge load support
     */
    //@{
    void giveEdgeDofMapping(IntArray &answer, int iEdge) const override;
    double computeEdgeVolumeAround(GaussPoint *gp, int iEdge) override;
    int computeLoadLEToLRotationMatrix(FloatMatrix &answer, int iEdge, GaussPoint *gp) override;
    //@}

public:
    // definition & identification
    const char *giveClassName() const override { return "QDKTPlate"; }
    const char *giveInputRecordName() const override { return _IFT_QDKTPlate_Name; }
    void initializeFrom(InputRecord &ir) override;

    int computeNumberOfDofs() override { return 12; }
    void giveDofManDofIDMask(int inode, IntArray &) const override;

    void computeMidPlaneNormal(FloatArray &answer, const GaussPoint *gp) override;

    double giveCharacteristicLength(const FloatArray &normalToCrackPlane) override;
    double computeVolumeAround(GaussPoint *gp) override;

    Interface *giveInterface(InterfaceType it) override;

    bool computeLocalCoordinates(FloatArray &answer, const FloatArray &gcoords) override;
    int giveIPValue(FloatArray &answer, GaussPoint *gp, InternalStateType type, TimeStep *tStep) override;

    void SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap) override;
    void SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap) override;
    int SPRNodalRecoveryMI_giveNumberOfIP() override { return this->numberOfGaussPoints; }
    SPRPatchType SPRNodalRecoveryMI_givePatchType() override;

    // layered cross section support functions
    void computeStrainVectorInLayer(FloatArray &answer, const FloatArray &masterGpStrain,
                                    GaussPoint *masterGp, GaussPoint *slaveGp, TimeStep *tStep) override;


};
} // end namespace oofem
#endif // qdkt_h
