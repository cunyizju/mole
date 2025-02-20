// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef SRC_SM_XFEM_PROPAGATIONLAWS_PLPRINCIPALSTRAIN_H_
#define SRC_SM_XFEM_PROPAGATIONLAWS_PLPRINCIPALSTRAIN_H_

#include "xfem/propagationlaw.h"

#define _IFT_PLPrincipalStrain_Name "propagationlawprincipalstrain"
#define _IFT_PLPrincipalStrain_Radius "radius" ///< Radius away from tip used when picking sampling point
//#define _IFT_PLHoopStressCirc_AngleInc "angleinc" ///< Angle between sampling points on the circle
#define _IFT_PLPrincipalStrain_IncLength "incrementlength" ///< Increment length per time step
#define _IFT_PLPrincipalStrain_StrainThreshold "strainthreshold" ///< Threshold for crack propagation
#define _IFT_PLPrincipalStrain_RadialBasisFunc "useradialbasisfunc" ///< If radial basis functions should be used for strain interpolation

namespace oofem {
class Domain;
class EnrichmentDomain;
class DynamicInputRecord;

class OOFEM_EXPORT PLPrincipalStrain : public PropagationLaw {
public:
    PLPrincipalStrain();
    virtual ~PLPrincipalStrain();

    const char *giveClassName() const override { return "PLPrincipalStrain"; }
    const char *giveInputRecordName() const override { return _IFT_PLPrincipalStrain_Name; }

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    bool hasPropagation() const override { return mIncrementLength > 0.; }
    bool propagateInterface(Domain &iDomain, EnrichmentFront &iEnrFront, TipPropagation &oTipProp) override;

    void setRadius(double iRadius) {mRadius = std::move(iRadius);}
    void setIncrementLength(double iIncrementLength) {mIncrementLength = std::move(iIncrementLength);}
    void setStrainThreshold(double iStrainThreshold) {mStrainThreshold = std::move(iStrainThreshold);}
    void setUseRadialBasisFunc(bool iUseRadialBasisFunc) {mUseRadialBasisFunc = std::move(iUseRadialBasisFunc);}

protected:
    double mRadius, mIncrementLength, mStrainThreshold;
    bool mUseRadialBasisFunc;

};
} // end namespace oofem

#endif /* SRC_SM_XFEM_PROPAGATIONLAWS_PLPRINCIPALSTRAIN_H_ */
