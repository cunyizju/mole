// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef linquad3d_planestress_h
#define linquad3d_planestress_h

#include "sm/Elements/PlaneStress/planstrss.h"
#include "sm/ErrorEstimators/directerrorindicatorrc.h"
#include "sm/ErrorEstimators/huertaerrorestimator.h"
#include "nodalrecovery/zznodalrecoverymodel.h"
#include "nodalrecovery/sprnodalrecoverymodel.h"
#include "mesher/spatiallocalizer.h"
#include "input/eleminterpmapperinterface.h"

#define _IFT_LinQuad3DPlaneStress_Name "linquad3dplanestress"

namespace oofem {
class FEI2dQuadLin;

/**
 * This class implements an isoparametric four-node quadrilateral plane-
 * stress elasticity finite element. Each node has 2 degrees of freedom.
 */
class LinQuad3DPlaneStress : public PlaneStress2d
{
protected:
    /// Local vertex coordinates
    std :: vector< FloatArray > lc; 
    /**
     * Transformation Matrix form GtoL(3,3) is stored
     * at the element level for computation efficiency
     */
    FloatMatrix *GtoLRotationMatrix;

    enum CharTensor {
        LocalStrainTensor,
        GlobalStrainTensor,
        LocalCurvatureTensor,
        GlobalCurvatureTensor,

        LocalForceTensor,
        GlobalForceTensor,
        LocalMomentTensor,
        GlobalMomentTensor
    };

public:
    LinQuad3DPlaneStress(int n, Domain * d);
    virtual ~LinQuad3DPlaneStress();

    Interface *giveInterface(InterfaceType it) override;
    FEICellGeometry* giveCellGeometryWrapper() override;
    void computeLocalNodalCoordinates(std::vector< FloatArray > &lxy);

    int computeNumberOfDofs() override { return 8; }
    int computeNumberOfGlobalDofs() override { return 12; }
    void giveDofManDofIDMask(int inode, IntArray &) const override;

    const FloatMatrix *computeGtoLRotationMatrix();
    bool computeGtoLRotationMatrix(FloatMatrix &answer) override;
    int computeLoadGToLRotationMtrx(FloatMatrix &answer) override;

    void printOutputAt(FILE *file, TimeStep *tStep) override;
    int giveIPValue(FloatArray &answer, GaussPoint *gp, InternalStateType type, TimeStep *tStep) override;
    void giveCharacteristicTensor(FloatMatrix &answer, CharTensor type, GaussPoint *gp, TimeStep *tStep);

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_LinQuad3DPlaneStress_Name; }
    const char *giveClassName() const override { return "LinQuad3DPlaneStress"; }
};
} // end namespace oofem
#endif // linquad3d_planestress_h
