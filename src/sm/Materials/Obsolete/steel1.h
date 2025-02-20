// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef steel1_h
#define steel1_h

#include "perfectlyplasticmaterial.h"
#include "sm/Materials/Elasticity/isolinearelasticmaterial.h"

///@name Input fields for Steel1
//@{
#define _IFT_Steel1_Name "steel1"
#define _IFT_Steel1_ry "ry"
//@}

namespace oofem {
class Domain;
/**
 * This class implements a isotropic perfectly plastic linear material in a finite
 * element problem.
 */
class Steel1 : public PerfectlyPlasticMaterial
{
public:
    Steel1(int n, Domain * d);

    void initializeFrom(InputRecord &ir) override;
    const char *giveInputRecordName() const override { return _IFT_Steel1_Name; }
    const char *giveClassName() const override { return "Steel1MaterialClass"; }
    void updateIfFailure(GaussPoint *gp, FloatArray *, FloatArray *) override { }
    bool hasCastingTimeSupport() const override { return true; }

protected:
    //
    // yield(YC-like functions) and loading(LC-like functions) criteria specific section
    //
    double      computeYCValueAt(GaussPoint *, FloatArray *, FloatArray *) override;
    FloatArray *GiveYCStressGradient(GaussPoint *, FloatArray *, FloatArray *) override;
    FloatArray *GiveLCStressGradient(GaussPoint *, FloatArray *, FloatArray *) override;
    FloatArray *GiveYCPlasticStrainGradient(GaussPoint *, FloatArray *, FloatArray *) override;
    FloatArray *GiveLCPlasticStrainGradient(GaussPoint *, FloatArray *, FloatArray *) override;
    void        updateTempYC(GaussPoint *, FloatArray *, FloatArray *) override { }
    void        updateTempLC(GaussPoint *, FloatArray *, FloatArray *) override { }
    // update during computation

    // auxiliary function
    double computeJ2InvariantAt(FloatArray *);
};
} // end namespace oofem
#endif // steel1_h
