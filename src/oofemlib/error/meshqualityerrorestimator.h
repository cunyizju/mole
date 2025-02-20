// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef meshqualityerrorestimator_h
#define meshqualityerrorestimator_h

#include "error/errorestimator.h"
#include "error/errorestimatortype.h"

#define _IFT_MeshQualityErrorEstimator_Name "meshquality"

namespace oofem {
class Domain;
class Element;
class TimeStep;
class IntegrationRule;
class FEInterpolation;
/**
 * This error estimator measures the quality of the elements.
 * This is useful when elements are deformed during the simulation.
 *
 * Error estimates should be unitless, positive, and scaled properly.
 * Zero error should reflect a perfect element.
 * Error value of one should reflect an element of acceptable quality.
 *
 * @note Its open for discussion whether the routine should give the reciprocal error to handle extremely badly shaped elements.
 *
 * @author Mikael Öhman
 */
class OOFEM_EXPORT MeshQualityErrorEstimator : public ErrorEstimator
{
protected:
    /**
     * Computes error based on the inscribed triangle/circle ratio.
     * @return Error value for the element. Zero for equilateral triangle.
     */
    static double computeTriangleRadiusError(Element *elem);

    /**
     * Computes the error based on the conditioning of the Jacobian.
     * @return Error value for the element.
     */
    static double computeJacobianError(FEInterpolation &fei, IntegrationRule &ir, Element *elem);

public:
    /// Constructor
    MeshQualityErrorEstimator(int n, Domain * d) : ErrorEstimator(n, d) {
        this->eeType = EET_MeshQuality;
    }
    /// Destructor
    virtual ~MeshQualityErrorEstimator() { }

    double giveElementError(EE_ErrorType type, Element *elem, TimeStep *tStep) override;

    /// Gives the max error from any element in the domain.
    double giveValue(EE_ValueType type, TimeStep *tStep) override;

    /// Empty implementation.
    int estimateError(EE_ErrorMode mode, TimeStep *tStep) override;

    RemeshingCriteria *giveRemeshingCrit() override { return nullptr; }

    void initializeFrom(InputRecord &ir) override;
    const char *giveClassName() const override { return "MeshQualityErrorEstimator"; }
    const char *giveInputRecordName() const override { return _IFT_MeshQualityErrorEstimator_Name; }
};
} // end namespace oofem
#endif // meshqualityerrorestimator_h
