// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef PLHOOPSTRESSCIRC_H_
#define PLHOOPSTRESSCIRC_H_

#include "xfem/propagationlaw.h"

#define _IFT_PLHoopStressCirc_Name "propagationlawhoopstresscirc"
#define _IFT_PLHoopStressCirc_Radius "radius" ///< Radius of circle used for stress sampling points
#define _IFT_PLHoopStressCirc_AngleInc "angleinc" ///< Angle between sampling points on the circle
#define _IFT_PLHoopStressCirc_IncLength "incrementlength" ///< Increment length per time step
#define _IFT_PLHoopStressCirc_HoopStressThreshold "hoopstressthreshold" ///< Threshold for crack propagation
#define _IFT_PLHoopStressCirc_RadialBasisFunc "useradialbasisfunc" ///< If radial basis functions should be used for stress interpolation

namespace oofem {
class Domain;
class EnrichmentDomain;
class DynamicInputRecord;


/**
 * Propagation law that propagates the crack in the direction
 * that gives @f$ \sigma_{r\theta} = 0 @f$.
 * Based on
 * T.P. Fries and M. Baydoun:
 * "Crack propagation with the extended finite element method
 * and a hybrid explicit-implicit crack description",
 * Internat. J. Numer. Methods Engrg 89,
 * pp. 1527--1558 (2012)
 *
 * The stress is evaluated in several points on a circle
 * surrounding the crack tip.
 *
 * Compared to the paper above, the implementation has been extended
 * with a criterion for crack propagation instead of always
 * propagating a predefined increment length. Two options are
 * currently available for stress interpolation:
 * 1) Take stress of closest Gauss point
 * 2) Interpolate with radial basis functions
 *
 * @author Erik Svenning
 */
class OOFEM_EXPORT PLHoopStressCirc : public PropagationLaw
{
public:
    PLHoopStressCirc() : mRadius(0.0), mAngleInc(0.0), mIncrementLength(0.0), mHoopStressThreshold(0.0), mUseRadialBasisFunc(false) { }
    virtual ~PLHoopStressCirc() { }

    const char *giveClassName() const override { return "PLHoopStressCirc"; }
    const char *giveInputRecordName() const override { return _IFT_PLHoopStressCirc_Name; }

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    bool hasPropagation() const override { return mIncrementLength > 0.; } ///@todo Could this be done smarter? / Mikael
    bool propagateInterface(Domain &iDomain, EnrichmentFront &iEnrFront, TipPropagation &oTipProp) override;

    void setRadius(double iRadius) {mRadius = std::move(iRadius);}
    void setAngleInc(double iAngleInc) {mAngleInc = std::move(iAngleInc);}
    void setIncrementLength(double iIncrementLength) {mIncrementLength = std::move(iIncrementLength);}
    void setHoopStressThreshold(double iHoopStressThreshold) {mHoopStressThreshold = std::move(iHoopStressThreshold);}
    void setUseRadialBasisFunc(bool iUseRadialBasisFunc) {mUseRadialBasisFunc = std::move(iUseRadialBasisFunc);}

protected:
    double mRadius, mAngleInc, mIncrementLength, mHoopStressThreshold;
    bool mUseRadialBasisFunc;
};
} // end namespace oofem


#endif /* PLHOOPSTRESSCIRC_H_ */
