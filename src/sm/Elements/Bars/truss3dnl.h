// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef truss3dnl_h
#define truss3dnl_h

#include "../sm/Elements/Bars/truss3d.h"

#define _IFT_Truss3dnl_Name "truss3dnl"


#define _IFT_Truss3dnl_initialStretch "initstretch"

namespace oofem {
/**
 * This class implements a nonlinear two-node truss bar element for three-dimensional
 * analysis.
 */
class Truss3dnl : public Truss3d
{
protected:
    double initialStretch;

public:
    Truss3dnl(int n, Domain * d);
    virtual ~Truss3dnl() { }

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_Truss3dnl_Name; }
    const char *giveClassName() const override { return "Truss3dnl"; }

    void initializeFrom(InputRecord &ir) override;
    void computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep) override;
    void giveInternalForcesVector(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord = 0) override;

protected:

    void computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, TimeStep *tStep, bool lin = false);
    void computeBlMatrixAt(GaussPoint *gp, FloatMatrix &answer);
    void computeBnlMatrixAt(GaussPoint *gp, FloatMatrix &answer, TimeStep *tStep, bool lin = false);
    void computeInitialStressStiffness(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep);
    void computeGaussPoints() override;


};
} // end namespace oofem
#endif // truss3dnl_h
