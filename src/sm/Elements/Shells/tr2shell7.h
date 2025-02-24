// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef Tr2Shell7_h
#define Tr2Shell7_h

#include "sm/Elements/Shells/shell7base.h"
#include "sm/CrossSections/layeredcrosssection.h"
#include "sm/Elements/nlstructuralelement.h"
#include "nodalrecovery/nodalaveragingrecoverymodel.h"

#define _IFT_Tr2Shell7_Name "tr2shell7"

namespace oofem {
class FEI3dTrQuad;
class BoundaryLoad;

/**
 * This class represent a 7 parameter shell element.
 * Each node has 7 degrees of freedom (displ. vec., director vec., inhomogeneous thickness strain ).
 * Nodal numbering:
 *           (3)
 *          / |
 *        /   |
 *      (6)  (5)
 *     /      |
 *   /        |
 * (1)--(4)--(2)
 * Add ref. to paper!
 * @author Jim Brouzoulis
 * @date 2012-11-01
 */

class Tr2Shell7 : public Shell7Base
{
protected:
    static FEI3dTrQuad interpolation; 
    static IntArray orderingDofTypes;
    static IntArray orderingNodes;
    static IntArray orderingEdgeNodes;

    const IntArray &giveOrderingDofTypes() const override;
    const IntArray &giveOrderingNodes() const override;
    const IntArray &giveOrderingEdgeNodes() const override;

    //specific
    void giveSurfaceDofMapping(IntArray &answer, int iSurf) const override;
    void giveEdgeDofMapping(IntArray &answer, int iEdge) const override;

    double computeVolumeAroundLayer(GaussPoint *mastergp, int layer) override;
    double computeAreaAround(GaussPoint *gp, double xi) override;
    void computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep) override
    { OOFEM_ERROR("calling of this function is not allowed"); }
    void computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override
    { OOFEM_ERROR("calling of this funciton is not allowed"); }

    void computeGaussPoints() override;
    //only used for debuging
    void compareMatrices(const FloatMatrix &matrix1, const FloatMatrix &matrix2, FloatMatrix &answer);

    FEInterpolation *giveInterpolation() const override;

public:
    Tr2Shell7(int n, Domain * d);        // constructor
    virtual ~Tr2Shell7() { }            // destructor -> declaring as virtual will make each subclass call their respective destr.
    // definition & identification
    int giveNumberOfDofs() override { return 42; }
    int giveNumberOfEdgeDofs() override { return 21; }
    int giveNumberOfEdgeDofManagers() override { return 3;  }
    const char *giveInputRecordName() const override { return _IFT_Tr2Shell7_Name; }
    const char *giveClassName() const override { return "Tr2Shell7"; }
    //Element_Geometry_Type giveGeometryType() const override { return EGT_triangle_2; }
    Element_Geometry_Type giveGeometryType() const override { return EGT_Composite; }
    integrationDomain giveIntegrationDomain() const override { return _Triangle; }     // write new wedge-like type 'layeredWedge'

    void giveBoundaryLocationArray(IntArray &locationArray, const IntArray &bNodes, const UnknownNumberingScheme &s, IntArray *dofIdArray) override;
};
} // end namespace oofem
#endif
