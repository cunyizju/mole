// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef ltrspaceboundarytruss_h
#define ltrspaceboundarytruss_h

#include "sm/Elements/3D/ltrspaceboundary.h"

#define _IFT_LTRSpaceBoundaryTruss_Name "ltrspaceboundarytruss"
#define _IFT_LTRSpaceBoundaryTruss_Location "location"

namespace oofem {
class FEI3dTetLin;

/**
 * This class implements a linear tetrahedral four-node finite element.
 * Each node has 3 degrees of freedom. This element is used for 3D RVE analyses with Periodic Boundary Conditions.
 * At least one node is located at the image boundary.
 * These nodes are replaced with a periodic mirror nodes and a control node is used to impose the macroscopic (average) strain.
 * MACROSCOPIC INPUT: Exx (1D, 1 COMPONENT)
 *
 * @author: Adam Sciegaj
 */
class LTRSpaceBoundaryTruss : public LTRSpaceBoundary
{
protected:
    void computeTransformationMatrix(FloatMatrix &answer, TimeStep *tStep) override;

public:
    LTRSpaceBoundaryTruss(int n, Domain *d);
    virtual ~LTRSpaceBoundaryTruss() { }

    int computeNumberOfDofs() override { return 13; };
    void giveDofManDofIDMask(int inode, IntArray &answer) const override;

    // definition & identification
    void initializeFrom(InputRecord &ir) override;
    const char *giveInputRecordName() const override { return _IFT_LTRSpaceBoundaryTruss_Name; }
    const char *giveClassName() const override { return "LTRSpaceBoundaryTruss"; }
};
} // end namespace oofem
#endif // LTRSpaceBoundaryTruss_h
