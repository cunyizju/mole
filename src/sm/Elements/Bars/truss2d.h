// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef truss2d_h
#define truss2d_h

#include "sm/Elements/nlstructuralelement.h"

///@name Input fields for 2D truss element
//@{
#define _IFT_Truss2d_Name "truss2d"
#define _IFT_Truss2d_cs "cs"
//@}

namespace oofem {
class FEI2dLineLin;
/**
 * This class implements a two-node truss bar element for two-dimensional
 * analysis.
 *
 * A truss bar element is characterized by its 'length' and its 'pitch'. The
 * pitch is the angle in radians between the X-axis and the axis of the
 * element (oriented node1 to node2).
 *
 * Can be used in xy, xz or yz planes.
 *
 * @author Peter Grassl
 */
class Truss2d : public NLStructuralElement
{
protected:
    double length;
    double pitch;
    int cs_mode;
    // array for diffrent cs_modes
    static FEI2dLineLin interp[ 3 ];  // only defined it so far...

public:
    Truss2d(int n, Domain *d);
    virtual ~Truss2d() { }

    void computeLumpedMassMatrix(FloatMatrix &answer, TimeStep *tStep) override;
    void computeMassMatrix(FloatMatrix &answer, TimeStep *tStep) override
    { computeLumpedMassMatrix(answer, tStep); }
    int giveLocalCoordinateSystem(FloatMatrix &answer) override;

    int computeGlobalCoordinates(FloatArray &answer, const FloatArray &lcoords) override;

    int computeNumberOfDofs() override { return 4; }
    void giveDofManDofIDMask(int inode, IntArray &answer) const override;

    void computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep) override;
    void computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override;
    void computeConstitutiveMatrix_dPdF_At(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override;

    // characteristic length (for crack band approach)
    double giveCharacteristicLength(const FloatArray &) override
    { return this->computeLength(); }

    double computeVolumeAround(GaussPoint *gp) override;

    int testElementExtension(ElementExtension ext) override { return ( ( ext == Element_EdgeLoadSupport ) ? 1 : 0 ); }

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_Truss2d_Name; }
    const char *giveClassName() const override { return "Truss2d"; }
    void initializeFrom(InputRecord &ir) override;
    ///@todo Introduce interpolator and remove these:
    Element_Geometry_Type giveGeometryType() const override { return EGT_line_1; }
    integrationDomain giveIntegrationDomain() const override { return _Line; }
    MaterialMode giveMaterialMode() override { return _1dMat; }
    FEInterpolation *giveInterpolation() const override;

protected:
    // edge load support
    void resolveCoordIndices(int &c1, int &c2);
    void giveEdgeDofMapping(IntArray &answer, int) const override;
    double computeEdgeVolumeAround(GaussPoint *gp, int) override;
    int computeLoadLEToLRotationMatrix(FloatMatrix &, int, GaussPoint *gp) override;
    void computeBmatrixAt(GaussPoint *gp, FloatMatrix &, int = 1, int = ALL_STRAINS) override;
    void computeBHmatrixAt(GaussPoint *gp, FloatMatrix &) override;
    void computeNmatrixAt(const FloatArray &iLocCoord, FloatMatrix &) override;
    void computeGaussPoints() override;

    double computeLength() override;
    double givePitch();
};
} // end namespace oofem
#endif // truss2d_h
