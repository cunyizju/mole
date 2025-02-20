// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef qplanstrssphf_h
#define qplanstrssphf_h

#include "sm/Elements/phasefieldelement.h"
#include "sm/Elements/PlaneStress/qplanstrss.h"

#define _IFT_QPlaneStressPhF2d_Name "QPlaneStressPhF2d"

namespace oofem {
/**
 * This class implements an Quadratic isoparametric eight-node quadrilateral plane-
 * stress phase field finite element. Each node has 3 degrees of freedom.
 * @todo in development
 */
class QPlaneStressPhF2d : public QPlaneStress2d, public PhaseFieldElement
{
public:
    QPlaneStressPhF2d(int n, Domain *d);
    virtual ~QPlaneStressPhF2d() { }

    NLStructuralElement *giveElement() override { return this; } 

    int computeNumberOfDofs() override { return 24; }
    void giveDofManDofIDMask( int inode, IntArray &answer ) const override;
    void giveDofManDofIDMask_u( IntArray &answer ) override;
    void giveDofManDofIDMask_d( IntArray &answer ) override;

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_QPlaneStressPhF2d_Name; }
    const char *giveClassName() const override { return "QPlaneStressPhF2d"; }

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
