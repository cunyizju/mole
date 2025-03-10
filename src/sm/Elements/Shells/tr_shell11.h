// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef tr_shell11_h
#define tr_shell11_h

#include "sm/Elements/structuralelement.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "sm/ErrorEstimators/zzerrorestimator.h"
#include "nodalrecovery/sprnodalrecoverymodel.h"
#include "sm/CrossSections/layeredcrosssection.h" 
#include "sm/Elements/Shells/cct3d.h"
#include "sm/Elements/PlaneStress/trplanrot3d.h"
#include "mesher/spatiallocalizer.h"
#include "fei/fei2dtrlin.h"
#include <memory>

#define _IFT_TR_SHELL11_Name "tr_shell11"

namespace oofem {

/**
 * This class implements an triangular three-node shell finite element, essentially a combination of 
 * cct3d and trplanrot3d elements. Each node has 6 degrees of freedom.
 *
 * @author Borek Patzak
 */
  class TR_SHELL11 : public StructuralElement, public ZZNodalRecoveryModelInterface, public NodalAveragingRecoveryModelInterface, public ZZErrorEstimatorInterface, public SPRNodalRecoveryModelInterface, public SpatialLocalizerInterface, public LayeredCrossSectionInterface
{
protected:
      static FEI2dTrLin interp_lin;
  double area;
  int numberOfRotGaussPoints;
  FloatMatrix GtoLRotationMatrix;
public:
    /// Constructor
    TR_SHELL11(int n, Domain * d);
    /// Destructor
    virtual ~TR_SHELL11() {}

    FEInterpolation *giveInterpolation() const override { return &interp_lin; }

    int computeNumberOfDofs() override { return 18; }
    void giveDofManDofIDMask(int inode, IntArray &answer) const override;
    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_TR_SHELL11_Name; }
    const char *giveClassName() const override { return "TR_SHELL11"; }
    void initializeFrom(InputRecord &ir) override;

    void computeGaussPoints() override;
    void computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int = 1, int = ALL_STRAINS) override;
    void computeNmatrixAt(const FloatArray &iLocCoord, FloatMatrix &answer) override;

    void computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override;
    void computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep) override;

    double giveArea();



  const FloatMatrix *computeGtoLRotationMatrix();
  bool computeGtoLRotationMatrix(FloatMatrix &answer) override;
  void giveLocalCoordinates(FloatArray &answer, const FloatArray &global);
  void giveNodeCoordinates(FloatArray &x, FloatArray &y) ;
  
  void giveCharacteristicTensor(FloatMatrix &answer, CharTensor type, GaussPoint *gp, TimeStep *tStep);
  int giveIPValue(FloatArray &answer, GaussPoint *gp, InternalStateType type, TimeStep *tStep) override;
  int computeLoadGToLRotationMtrx(FloatMatrix &answer) override;
  void computeBodyLoadVectorAt(FloatArray &answer, Load *forLoad, TimeStep *tStep, ValueModeType mode) override;

  void computeSurfaceNMatrixAt(FloatMatrix &answer, int iSurf, GaussPoint *gp);
  void giveSurfaceDofMapping(IntArray &answer, int iSurf) const override;
  double computeSurfaceVolumeAround(GaussPoint *gp, int iSurf) override;
  int computeLoadLSToLRotationMatrix(FloatMatrix &answer, int iSurf, GaussPoint *gp) override;
  
  double computeVolumeAround(GaussPoint *gp) override;
  double giveCharacteristicLength(const FloatArray &normalToCrackPlane) override;
  
  void printOutputAt(FILE *file, TimeStep *tStep) override;

  Element_Geometry_Type giveGeometryType() const override { return EGT_triangle_1; }
  integrationDomain giveIntegrationDomain() const override { return _Triangle; }
  MaterialMode giveMaterialMode() override { return _3dShellRot; }
  
  Interface *giveInterface(InterfaceType it) override;
  
  void NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node,
						  InternalStateType type, TimeStep *tStep) override;
  
  void SPRNodalRecoveryMI_giveSPRAssemblyPoints(IntArray &pap) override;
  void SPRNodalRecoveryMI_giveDofMansDeterminedByPatch(IntArray &answer, int pap) override;
  int SPRNodalRecoveryMI_giveNumberOfIP() override { return 4; }
  SPRPatchType SPRNodalRecoveryMI_givePatchType() override;
  

  // layered cross section support functions
  void computeStrainVectorInLayer(FloatArray &answer, const FloatArray &masterGpStrain,
				  GaussPoint *masterGp, GaussPoint *slaveGp, TimeStep *tStep) override;

  
protected:
  FloatArray GiveDerivativeUX(const FloatArray &lCoords);
    FloatArray GiveDerivativeVX(const FloatArray &lCoords);
    FloatArray GiveDerivativeUY(const FloatArray &lCoords);
    FloatArray GiveDerivativeVY(const FloatArray &lCoords);
    FloatArray GivePitch();
};
}// end namespace oofem
#endif
