// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef QTRPLANSTRSSXFEM_H_
#define QTRPLANSTRSSXFEM_H_

#include "sm/Elements/PlaneStress/qtrplstr.h"
#include "sm/xfem/xfemstructuralelementinterface.h"
#include "export/vtkxmlexportmodule.h"

#include "input/domain.h"

#define _IFT_QTrPlaneStress2dXFEM_Name "qtrplanestress2dxfem"
#define _IFT_QTrPlaneStress2dXFEM_RegCoeff "reg_coeff"
#define _IFT_QTrPlaneStress2dXFEM_RegCoeffTol "reg_coeff_tol"

namespace oofem {


/**
 * 6-node triangle with XFEM kinematics.
 * @author Erik Svenning
 * @date Dec 19, 2014
 */
class QTrPlaneStress2dXFEM : public QTrPlaneStress2d, public XfemStructuralElementInterface, public VTKXMLExportModuleElementInterface
{
protected:
    void updateYourself(TimeStep *tStep) override;
    void postInitialize() override;

    double mRegCoeff, mRegCoeffTol;

public:
    QTrPlaneStress2dXFEM(int n, Domain * d) : QTrPlaneStress2d(n, d), XfemStructuralElementInterface(this), VTKXMLExportModuleElementInterface() { numberOfDofMans = 6; mRegCoeff = 1.0e-6; mRegCoeffTol = 1.0e-6; }
    virtual ~QTrPlaneStress2dXFEM();

    Interface *giveInterface(InterfaceType it) override;

    const char *giveInputRecordName() const override { return _IFT_QTrPlaneStress2dXFEM_Name; }
    const char *giveClassName() const override { return "QTrPlaneStress2dXFEM"; }

    int computeNumberOfDofs() override;
    void computeGaussPoints() override;
    void computeNmatrixAt(const FloatArray &iLocCoord, FloatMatrix &answer) override;
    void computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer,
                          int lowerIndx = 1, int upperIndx = ALL_STRAINS) override;
    void computeBHmatrixAt(GaussPoint *gp, FloatMatrix &answer) override;

    void giveDofManDofIDMask(int inode, IntArray &answer) const override;
    void computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *, TimeStep *tStep) override;
    void computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep) override;
    void computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep) override;

    void computeDeformationGradientVector(FloatArray &answer, GaussPoint *gp, TimeStep *tStep) override;

    void giveInternalForcesVector(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord) override;
    void computeConsistentMassMatrix(FloatMatrix &answer, TimeStep *tStep, double &mass, const double *ipDensity = NULL) override { XfemStructuralElementInterface :: XfemElementInterface_computeConsistentMassMatrix(answer, tStep, mass, ipDensity); }

    Element_Geometry_Type giveGeometryType() const override;

    void initializeFrom(InputRecord &ir) override;
    MaterialMode giveMaterialMode() override;
    void giveInputRecord(DynamicInputRecord &input) override;

    void computeField(ValueModeType mode, TimeStep *tStep, const FloatArray &lcoords, FloatArray &answer) override;

    /// VTK Interface
    void giveCompositeExportData(std::vector< ExportRegion > &vtkPieces, IntArray &primaryVarsToExport, IntArray &internalVarsToExport, IntArray cellVarsToExport, TimeStep *tStep) override;

protected:
    int giveNumberOfIPForMassMtrxIntegration() override { return 6; } // TODO: Check
};

} /* namespace OOFEM */
#endif /* QTRPLANSTRSSXFEM_H_ */
