// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef scalarerrorindicator_h
#define scalarerrorindicator_h

#include "error/errorestimator.h"
#include "input/internalstatetype.h"

///@name Input fields for ScalarErrorIndicator
//@{
#define _IFT_ScalarErrorIndicator_Name "scalar"
#define _IFT_ScalarErrorIndicator_vartype "vartype"
//@}

namespace oofem {
class RemeshingCriteria;

/**
 * The class representing scalar error indicator.
 * It indicates element error based on the value of some suitable scalar value obtained from the
 * element integration points and corresponding material model.
 */
class ScalarErrorIndicator : public ErrorEstimator
{
protected:
    /// Type of internal variable to be indicator (type for temp and nontemp version).
    int indicatorType;
    /// Corresponding internal state type.
    InternalStateType varType;

public:
    /// Constructor
    ScalarErrorIndicator(int n, Domain * d) : ErrorEstimator(n, d) {
        eeType = EET_SEI;
    }
    /// Destructor
    virtual ~ScalarErrorIndicator() { }

    double giveElementError(EE_ErrorType type, Element *elem, TimeStep *tStep) override;
    double giveValue(EE_ValueType type, TimeStep *tStep) override { return 0.0; }
    int estimateError(EE_ErrorMode mode, TimeStep *tStep) override;
    RemeshingCriteria *giveRemeshingCrit() override;

    void initializeFrom(InputRecord &ir) override;

    const char *giveClassName() const override { return "ScalarErrorIndicator"; }
    const char *giveInputRecordName() const override { return _IFT_ScalarErrorIndicator_Name; }
};
} // end namespace oofem
#endif // scalarerrorindicator_h
