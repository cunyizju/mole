// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef libeam3d_h
#define libeam3d_h

#include "sm/Elements/structuralelement.h"
#include "sm/CrossSections/fiberedcs.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"
#include "dofman/node.h"

///@name Input fields for LIBeam3d
//@{
#define _IFT_LIBeam3d_Name "libeam3d"
#define _IFT_LIBeam3d_refnode "refnode"
//@}

namespace oofem {
/**
 * This class implements a 3-dimensional mindlin theory Linear Isoparametric
 * beam element, with reduced integration.
 */
class LIBeam3d : public StructuralElement, public FiberedCrossSectionInterface, public NodalAveragingRecoveryModelInterface
{
private:
    double length;
    int referenceNode;

public:
    LIBeam3d(int n, Domain * d);
    virtual ~LIBeam3d() { }

    void initializeFrom(InputRecord &ir) override;

    void computeLumpedMassMatrix(FloatMatrix &answer, TimeStep *tStep) override;
    void computeMassMatrix(FloatMatrix &answer, TimeStep *tStep) override
    { computeLumpedMassMatrix(answer, tStep); }
    void computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep) override;
    bool computeGtoLRotationMatrix(FloatMatrix &answer) override;

    int testElementExtension(ElementExtension ext) override;

    int computeNumberOfDofs() override { return 12; }
    void giveDofManDofIDMask(int inode, IntArray &answer) const override;
    double computeVolumeAround(GaussPoint *gp) override;
    int giveLocalCoordinateSystem(FloatMatrix &answer) override;
    Node* giveReferenceNode(int refNode);
    int giveIPValue(FloatArray &answer, GaussPoint *gp, InternalStateType type, TimeStep *tStep) override;
    int computeGlobalCoordinates(FloatArray &answer, const FloatArray &lcoords) override;

    bool isCast(TimeStep *tStep) override {return true;}
    Element_Geometry_Type giveGeometryType() const override { return EGT_line_1; }

    // Fibered cross section support functions
    void FiberedCrossSectionInterface_computeStrainVectorInFiber(FloatArray &answer, const FloatArray &masterGpStrain,
                                                                 GaussPoint *slaveGp, TimeStep *tStep) override;

    void NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node,
                                                            InternalStateType type, TimeStep *tStep) override;

    Interface *giveInterface(InterfaceType it) override;

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_LIBeam3d_Name; }
    const char *giveClassName() const override { return "LIBeam3d"; }

    integrationDomain giveIntegrationDomain() const override { return _Line; }
    MaterialMode giveMaterialMode() override { return _3dBeam; }

protected:
    // edge load support
    void giveEdgeDofMapping(IntArray &answer, int iEdge) const override;
    double computeEdgeVolumeAround(GaussPoint *gp, int iEdge) override;
    int computeLoadLEToLRotationMatrix(FloatMatrix &answer, int, GaussPoint *gp) override;
    int computeLoadGToLRotationMtrx(FloatMatrix &answer) override;
    void computeBodyLoadVectorAt(FloatArray &answer, Load *load, TimeStep *tStep, ValueModeType mode) override;

    void computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int = 1, int = ALL_STRAINS) override;
    void computeNmatrixAt(const FloatArray &iLocCoord, FloatMatrix &answer) override;
    void computeGaussPoints() override;
    void computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override;
    void computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep) override;

    double computeLength() override;
};
} // end namespace oofem
#endif // libeam3d_h
