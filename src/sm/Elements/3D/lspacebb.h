// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef lspacebb_h
#define lspacebb_h

#include "sm/Elements/3D/lspace.h"

#define _IFT_LSpaceBB_Name "lspacebb"

namespace oofem {
/**
 * Three dimensional brick with linear approximation, suitable for incompressible settings
 * This is achieved by selective integration of deviatoric (full integration) and
 * volumetric (one point) strain contributions. Implemented using bbar technique.
 */
class LSpaceBB  : public LSpace
{
public:
    LSpaceBB(int n, Domain * d);
    virtual ~LSpaceBB() { }

    const char *giveInputRecordName() const override { return _IFT_LSpaceBB_Name; }
    const char *giveClassName() const override { return "LSpaceBB"; }

protected:
    void computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int = 1, int = ALL_STRAINS) override;
};
} // end namespace oofem
#endif // lspacebb_h
