// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef cct3d_h
#define cct3d_h

#include "sm/Elements/Plates/cct.h"
#include "math/floatmatrix.h"

#define _IFT_CCTPlate3d_Name "cctplate3d"

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
 * This class represent CCT plate element that can be arbitrary
 * oriented in space, in contrast to base CCT element that is
 * defined in xy plane.
 * @see CCTPlate
 *
 * This class implements a triangular three-node plate CCT finite element.
 * Each node has 3 degrees of freedom.
 *
 * @author Ladislav Svoboda
 * @date 2010-5-25
 */
class CCTPlate3d : public CCTPlate
{
protected:
    /**
     * Transformation Matrix form GtoL(3,3) is stored
     * at the element level for computation efficiency.
     */
    FloatMatrix GtoLRotationMatrix;

public:
    CCTPlate3d(int n, Domain * d);
    virtual ~CCTPlate3d() {}

protected:
    void giveLocalCoordinates(FloatArray &answer, const FloatArray &global);
    void giveNodeCoordinates(double &x1, double &x2, double &x3,
                             double &y1, double &y2, double &y3,
                             double &z1, double &z2, double &z3) override;

    void giveCharacteristicTensor(FloatMatrix &answer, CharTensor type, GaussPoint *gp, TimeStep *tStep);
    int giveIPValue(FloatArray &answer, GaussPoint *gp, InternalStateType type, TimeStep *tStep) override;
    int computeLoadGToLRotationMtrx(FloatMatrix &answer) override;
    void computeBodyLoadVectorAt(FloatArray &answer, Load *forLoad, TimeStep *tStep, ValueModeType mode) override;

    friend class TR_SHELL01;
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
    const char *giveClassName() const override { return "CCTPlate3d"; }
    const char *giveInputRecordName() const override { return _IFT_CCTPlate3d_Name; }

    int computeNumberOfDofs() override { return 9; }
    int computeNumberOfGlobalDofs() override { return 18; }
    void giveDofManDofIDMask(int inode, IntArray &) const override;

    virtual const FloatMatrix *computeGtoLRotationMatrix();
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
#endif // cct3d_h
