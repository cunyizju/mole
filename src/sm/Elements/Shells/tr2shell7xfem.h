// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef Tr2Shell7XFEM_h
#define Tr2Shell7XFEM_h

#include "sm/Elements/Shells/shell7basexfem.h"
#include "sm/CrossSections/layeredcrosssection.h"
#include "sm/Elements/nlstructuralelement.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"

#define _IFT_Tr2Shell7XFEM_Name "tr2shell7xfem"

namespace oofem {
class FEI3dTrQuad;
class BoundaryLoad;

/**
 * This class represent a 7 parameter shell element.
 * Each node has 7 degrees of freedom (displ. vec., director vec., inhomogeneous thickness strain ).
 * Add ref. to paper!
 * @author Jim Brouzoulis
 * @date 2012-11-01
 */
class Tr2Shell7XFEM : public Shell7BaseXFEM
{
protected:
    static FEI3dTrQuad interpolation;
    static IntArray orderingDofTypes;
    static IntArray orderingNodes;
    static IntArray orderingEdgeNodes;

    const IntArray &giveOrderingDofTypes() const override;
    const IntArray &giveOrderingNodes() const override;
    const IntArray &giveOrderingEdgeNodes() const override;
    void giveSurfaceDofMapping(IntArray &answer, int iSurf) const override;
    void giveEdgeDofMapping(IntArray &answer, int iEdge) const override;

    double computeVolumeAroundLayer(GaussPoint *mastergp, int layer) override;
    double computeAreaAround(GaussPoint *gp, double xi) override;

    void computeGaussPoints() override;
    bool updateIntegrationRuleMultiCrack();
    void computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep) override
    { OOFEM_ERROR("calling of this function is not allowed"); }
    void computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override
    { OOFEM_ERROR("calling of this funciton is not allowed"); }

    virtual void giveLocalNodeCoords(FloatArray &nodeLocalXiCoords, FloatArray &nodeLocalEtaCoords);

    FEInterpolation *giveInterpolation() const override;
    double computeArea() override;
    // VTK
    void vtkGiveUpdatedFictiousNodeCoords(FloatArray nodeCoords [ 15 ], int layer, TimeStep *tStep);

public:
    Tr2Shell7XFEM(int n, Domain * d);
    virtual ~Tr2Shell7XFEM() { }     // destructor -> declaring as virtual will make each subclass call their respective destr.
    // definition & identification
    int giveNumberOfEdgeDofs() override { return 21; }
    int giveNumberOfEdgeDofManagers() override { return 3;  }
    const char *giveInputRecordName() const override { return _IFT_Tr2Shell7XFEM_Name; }
    const char *giveClassName() const override { return "Tr2Shell7XFEM"; }

    Element_Geometry_Type giveGeometryType() const override { return EGT_Composite; }
    integrationDomain giveIntegrationDomain() const override { return _Triangle; } // write new wedge-like type 'layeredWedge'
};
} // end namespace oofem
#endif
