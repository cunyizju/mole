// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef mitc4_h
#define mitc4_h

#include "sm/Elements/structuralelement.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "nodalrecovery/sprnodalrecoverymodel.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"
#include "mesher/spatiallocalizer.h"
#include "bc/load.h"
#include "math/floatmatrixf.h"
//#include "input/eleminterpmapperinterface.h"//

#define _IFT_MITC4Shell_Name "mitc4shell"
#define _IFT_MITC4Shell_nipZ "nipz"
#define _IFT_MITC4Shell_directorType "directortype"

namespace oofem {
class FEI2dQuadLin;
#ifndef __CHARTENSOR
 #define __CHARTENSOR
enum CharTensor {
    LocalStrainTensor,
    GlobalStrainTensor,
    LocalForceTensor,
    GlobalForceTensor,
};
#endif

/**
 * This class implements an quad element based on Mixed Interpolation of Tensorial Components (MITC).
 * This element is a shell element suitable for both thin and thick shells.
 * The element has 24 DOFs (u,v,w-displacements and three rotations) in each node
 *
 * Tasks:
 * - calculating its B,D matrices and dV.
 */

class MITC4Shell : public StructuralElement, public ZZNodalRecoveryModelInterface,
    public SPRNodalRecoveryModelInterface, public NodalAveragingRecoveryModelInterface,
    public SpatialLocalizerInterface
{
protected:
    /// Element geometry approximation
    static FEI2dQuadLin interp_lin;
    /**
     * Transformation Matrix form GtoL(3,3) is stored
     * at the element level for computation efficiency.
     */
    FloatMatrixF< 3, 3 >GtoLRotationMatrix;
    int nPointsXY = 0, nPointsZ = 0, directorType = 0;
    double drillCoeff = 0.;

public:
    MITC4Shell(int n, Domain *d);

    FEInterpolation *giveInterpolation() const override;
    FEInterpolation *giveInterpolation(DofIDItem id) const override;
    int testElementExtension(ElementExtension ext) override { return ( ( ( ext == Element_EdgeLoadSupport ) || ( ext == Element_SurfaceLoadSupport ) ) ? 1 : 0 ); }

    Interface *giveInterface(InterfaceType interface) override;
    // definition & identification
    const char *giveClassName() const override { return "MITC4Shell"; }
    const char *giveInputRecordName() const override { return _IFT_MITC4Shell_Name; }
    integrationDomain giveIntegrationDomain() const override { return _3dDegShell; }
    MaterialMode giveMaterialMode() override { return _3dDegeneratedShell; }
    void initializeFrom(InputRecord &ir) override;
    void postInitialize() override;
    int computeNumberOfDofs() override { return 24; }
    int computeNumberOfGlobalDofs() override { return 24; }

    void SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap) override;
    void SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap) override;
    int SPRNodalRecoveryMI_giveNumberOfIP() override;
    SPRPatchType SPRNodalRecoveryMI_givePatchType() override;
    void NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node, InternalStateType type, TimeStep *tStep) override;
    void giveInternalForcesVector(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord) override;

    // transformation
    bool computeGtoLRotationMatrix(FloatMatrix &answer) override;
    int computeLoadGToLRotationMtrx(FloatMatrix &answer) override;
    std::array< FloatMatrixF< 3, 3 >, 4 >computeLToDirectorRotationMatrix();
    int computeLoadLEToLRotationMatrix(FloatMatrix &answer, int iEdge, GaussPoint *gp) override;


protected:
    void computeGaussPoints() override;
    void computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep) override;
    void computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int = 1, int = ALL_STRAINS) override;
    void computeNmatrixAt(const FloatArray &iLocCoord, FloatMatrix &answer) override;
    void computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override;
    void computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep) override;


private:
    std::array< FloatArrayF< 3 >, 4 >giveNodeCoordinates();
    std::array< FloatArrayF< 3 >, 4 >giveDirectorVectors();
    std::array< FloatArrayF< 3 >, 4 >giveLocalDirectorVectors();
    std::array< double, 4 >giveThickness();
    FloatMatrixF< 3, 3 >giveJacobian(const FloatArrayF< 3 > &lcoords);
    FloatArrayF< 3 >giveLocalCoordinates(const FloatArrayF< 3 > &global);
    int giveIPValue(FloatArray &answer, GaussPoint *gp, InternalStateType type, TimeStep *tStep) override;
    FloatMatrix giveCharacteristicTensor(CharTensor type, GaussPoint *gp, TimeStep *tStep);
    void printOutputAt(FILE *file, TimeStep *tStep) override;
    int computeGlobalCoordinates(FloatArray &answer, const FloatArray &lcoords) override;
    bool computeLocalCoordinates(FloatArray &answer, const FloatArray &coords) override;
    double computeVolumeAround(GaussPoint *gp) override;
    std::array< FloatArrayF< 3 >, 3 >computeLocalBaseVectors();
    std::array< FloatArrayF< 4 >, 2 >givedNdx(const FloatArrayF< 3 > &coords);

    FloatArray giveMidplaneIPValue(int gpXY, InternalStateType type, TimeStep *tStep);

    // edge & body load
    double computeEdgeVolumeAround(GaussPoint *gp, int iEdge) override;
    void giveEdgeDofMapping(IntArray &answer, int iEdge) const override;
    void giveDofManDofIDMask(int inode, IntArray &) const override;
    double computeSurfaceVolumeAround(GaussPoint *gp, int iSurf) override;
    void computeSurfaceNMatrixAt(FloatMatrix &answer, int iSurf, GaussPoint *sgp);  ///@note This method isn't overloaded, it's never called for any class
    void giveSurfaceDofMapping(IntArray &answer, int iSurf) const override;

    void computeSurfaceNMatrix(FloatMatrix &answer, int boundaryID, const FloatArray &lcoords) override;
    void computeEdgeNMatrix(FloatMatrix &answer, int boundaryID, const FloatArray &lcoords) override;
    void setupIRForMassMtrxIntegration(IntegrationRule &iRule) override;
};
} // end namespace oofem
#endif // mitc4_h
