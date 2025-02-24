// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef solidshell_h
#define solidshell_h

#include "sm/Elements/3D/lspace.h"


#define _IFT_SolidShell_Name "solidshell"

// EAS parameters
#define _IFT_SolidShell_EAS_type "eas_type"


namespace oofem {
class FEI3dHexaLin;

/**
 * This class implements a Linear 8-noded shell like solid with ANS and EAS to remove and reduce certain locking aspects
 * Each node has 3 degrees of freedom.
 *
 *@author Jim Brouzoulis 
 */
class SolidShell  : public LSpace
{
protected:
    static FEI3dHexaLin interpolation;

public:
    SolidShell(int n, Domain * d);
    virtual ~SolidShell() { }
    FEInterpolation *giveInterpolation() const override;
    void computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int lowerIndx = 1, int upperIndx = ALL_STRAINS) override;
    void computeBHmatrixAt(GaussPoint *gp, FloatMatrix &answer) override;
    void computeBHmatrixAt(FloatArray &lCoords, FloatMatrix &answer);

    virtual void computeBEmatrixAt(GaussPoint *gp, FloatMatrix &answer, TimeStep *tStep);
    void computeGaussPoints() override;
    virtual void computeEASBmatrixAt(GaussPoint *gp, FloatMatrix &answer);
    void giveInternalForcesVector(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord) override;
    void computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep) override;
    void computeGeometricStiffness(FloatMatrix &answer, GaussPoint *gp, TimeStep *tStep);

    void computeFVector(FloatArray &answer, FloatArray &lCoords, FloatArray &ae);
    void computeEVector(FloatArray &answer, FloatArray &lCoords, FloatArray &ae);
    void computeEASfield(FloatArray &answer, TimeStep *tStep);

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_LSpace_Name; }
    const char *giveClassName() const override { return "SolidShell"; }
    void initializeFrom(InputRecord &ir) override;

    // variables associated with EAS
    int EAS_type; 
    FloatArray tempAlpha;
    FloatArray alpha;
    FloatArray u_k;
    FloatMatrix invKEE;
    FloatMatrix KEC;
    FloatArray fE;
    void computeAlpha(FloatArray &answer, FloatArray &u);

    void computeBondTransformationMatrix(FloatMatrix &answer, FloatMatrix &base);
    void postInitialize() override;
    int checkConsistency() override{ return 1; };

private:
    void x(int arg1);
};
} // end namespace oofem
#endif
