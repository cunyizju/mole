// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef dkt3d_h
#define dkt3d_h

#include "dkt.h"
#include "math/floatmatrix.h"

#define _IFT_DKTPlate3d_Name "dktplate3d"

namespace oofem {
#ifndef __CHARTENSOR
 #define __CHARTENSOR
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
#endif


/**
 * This class represent DKT plate element that can be arbitrary
 * oriented in space, in contrast to base DKT element that is
 * defined in xy plane.
 * @see DKTPlate
 *
 * This class implements a triangular three-node plate DKT finite element.
 * Each node has 3 degrees of freedom.
 *
 */
class DKTPlate3d : public DKTPlate
{
protected:
    /**
     * Transformation Matrix form GtoL(3,3) is stored
     * at the element level for computation efficiency.
     */
    FloatMatrix GtoLRotationMatrix;

public:
    DKTPlate3d(int n, Domain * d);
    virtual ~DKTPlate3d() {}

protected:
    void giveLocalCoordinates(FloatArray &answer, const FloatArray &global);
    void giveNodeCoordinates(double &x1, double &x2, double &x3,
                             double &y1, double &y2, double &y3,
                             double &z1, double &z2, double &z3) override;

    void giveCharacteristicTensor(FloatMatrix &answer, CharTensor type, GaussPoint *gp, TimeStep *tStep);
    int computeLoadGToLRotationMtrx(FloatMatrix &answer) override;
    void computeBodyLoadVectorAt(FloatArray &answer, Load *forLoad, TimeStep *tStep, ValueModeType mode) override;

    /**
     * @name Edge load support
     */
    //@{
    void giveEdgeDofMapping(IntArray &answer, int iEdge) const override;
    double computeEdgeVolumeAround(GaussPoint *gp, int iEdge) override;
    int computeLoadLEToLRotationMatrix(FloatMatrix &answer, int iEdge, GaussPoint *gp) override;
    //@}
    /**
     * @name Surface load support
     */
    //@{
    void computeSurfaceNMatrixAt(FloatMatrix &answer, int iSurf, GaussPoint *gp);
    void giveSurfaceDofMapping(IntArray &answer, int iSurf) const override;
    double computeSurfaceVolumeAround(GaussPoint *gp, int iSurf) override;
    int computeLoadLSToLRotationMatrix(FloatMatrix &answer, int iSurf, GaussPoint *gp) override;
    //@}

public:
    // definition & identification
    const char *giveClassName() const override { return "DktPlate3d"; }
    const char *giveInputRecordName() const override { return _IFT_DKTPlate3d_Name; }

    int computeNumberOfDofs() override { return 9; }
    int computeNumberOfGlobalDofs() override { return 18; }
    void giveDofManDofIDMask(int inode, IntArray &) const override;
    int giveIPValue(FloatArray &answer, GaussPoint *gp, InternalStateType type, TimeStep *tStep) override;

    const FloatMatrix *computeGtoLRotationMatrix();
    bool computeGtoLRotationMatrix(FloatMatrix &answer) override;

    bool computeLocalCoordinates(FloatArray &answer, const FloatArray &gcoords) override;
    int computeGlobalCoordinates(FloatArray &answer, const FloatArray &lcoords) override;

    int giveLocalCoordinateSystem(FloatMatrix &answer) override
    {
        OOFEM_ERROR("calling of this function id not allowed");
        return 0;
    }
    int testElementExtension(ElementExtension ext) override
    { return ( ( ext == Element_SurfaceLoadSupport )  ? 1 : 0 ); }

    void printOutputAt(FILE *file, TimeStep *tStep) override;
};
} // end namespace oofem
#endif // dkt3d_h
