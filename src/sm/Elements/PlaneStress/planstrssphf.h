// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef planstrssphf_h
#define planstrssphf_h

#include "sm/Elements/phasefieldelement.h"
#include "sm/Elements/PlaneStress/planstrss.h"

#define _IFT_PlaneStressPhF2d_Name "PlaneStressPhF2d"

namespace oofem {
/**
 * This class implements an isoparametric four-node quadrilateral plane-
 * stress phase field finite element. Each node has 3 degrees of freedom.
 * @todo in development
 */
class PlaneStressPhF2d : public PlaneStress2d, public PhaseFieldElement
{
protected:

public:
    PlaneStressPhF2d(int n, Domain *d);
    virtual ~PlaneStressPhF2d() { }

    NLStructuralElement *giveElement() override { return this; } 
    int computeNumberOfDofs() override { return 12; }
    void giveDofManDofIDMask(int inode, IntArray &answer) const override;
    void giveDofManDofIDMask_u(IntArray &answer) override;
    void giveDofManDofIDMask_d(IntArray &answer) override;

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_PlaneStressPhF2d_Name; }
    const char *giveClassName() const override { return "PlaneStressPhF2d"; }

    void computeStiffnessMatrix( FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep ) override
    {
        PhaseFieldElement :: computeStiffnessMatrix( answer, rMode, tStep );
    }
    void giveInternalForcesVector( FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord = 0 ) override 
    {
        PhaseFieldElement :: giveInternalForcesVector( answer, tStep, useUpdatedGpRecord );
    }
};
} // end namespace oofem
#endif // qplanstrss_h
