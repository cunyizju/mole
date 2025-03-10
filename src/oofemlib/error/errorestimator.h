// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li


//   *****************************************
//   *** CLASS ERROR ESTIMATOR (INDICATOR) ***
//   *****************************************

#ifndef errorestimator_h
#define errorestimator_h

#include "input/femcmpnn.h"
#include "utility/interface.h"
#include "error/errorestimatortype.h"
#include "math/intarray.h"
#include "input/internalstatetype.h"
#include "mesher/remeshingcrit.h"

#include <memory>

///@name Input fields for ErrorEstimator
//@{
#define _IFT_ErrorEstimator_regionskipmap "regionskipmap"
#define _IFT_ErrorEstimator_IStype "istype"
//@}

namespace oofem {
class Domain;
class Element;
class TimeStep;
class RemeshingCriteria;

/** Type characterizing different type of errors. */
enum EE_ValueType { relativeErrorEstimateEEV, globalNormEEV, globalErrorEEV, globalWeightedErrorEEV };
/** Type characterizing different type of element errors. */
enum EE_ErrorType { unknownET, indicatorET, internalStressET, primaryUnknownET };
/** Type determining whether temporary or equilibrated variables are used for error evaluation. */
enum EE_ErrorMode { equilibratedEM, temporaryEM };


/**
 * The base class for all error estimation or error indicator algorithms.
 * The basic task is to evaluate the error on associated domain. If this task requires
 * the special element algorithms, these should be included using interface concept.
 *
 * This estimator should also provide the compatible Remeshing Criteria class, which
 * based on various error measures will evaluate the required mesh density of a new domain.
 */
class OOFEM_EXPORT ErrorEstimator : public FEMComponent
{
protected:
    ErrorEstimatorType eeType;
    std :: unique_ptr< RemeshingCriteria > rc;
    /**
     * Map indicating regions to skip (region - cross section model).
     * Do not access this variable directly, since this variable is read from input and could have size different
     * from actual number of regions - use always the skipRegion method, since it performs size check.  and handles
     * all regions correctly.
     */
    IntArray regionSkipMap;
    /// Number of skipped elements.
    int skippedNelems;
    /// Internal state type of variable to get internal forces
    InternalStateType IStype;

public:
    /// Constructor
    ErrorEstimator(int n, Domain * d);
    /// Destructor
    virtual ~ErrorEstimator();
    /// Sets Domain; should also re-initialize attributes if necessary.
    void setDomain(Domain *d) override;
    /**
     * Returns the element error. The estimateError service should be called before.
     * @param type Error type.
     * @param elem Element for which error requested.
     * @param tStep Time step.
     * @return Element error.
     */
    virtual double giveElementError(EE_ErrorType type, Element *elem, TimeStep *tStep) = 0;
    /**
     * Returns the characteristic value of given type. The estimateError service should be called before.
     * This method is supposed to be used by associated remeshingCriteria to access some characteristic
     * values already computed or known at error estimator level.
     * @param type Error type.
     * @param tStep Time step.
     * @return Error value for given type.
     */
    virtual double giveValue(EE_ValueType type, TimeStep *tStep) = 0;
    /**
     * Returns number of elements skipped in error estimation.
     * @return Number of skipped elements.
     */
    int giveNumberOfSkippedElements() { return skippedNelems; }
    /**
     * Estimates the error on associated domain at given time step. The estimated values can be
     * requested using giveElementError and giveValue methods. The type of errors provided
     * depends on error estimator type implementing the service.
     * @param mode Error mode.
     * @param tStep Time step.
     */
    virtual int estimateError(EE_ErrorMode mode, TimeStep *tStep) = 0;
    /**
     * Returns reference to associated remeshing criteria.
     */
    virtual RemeshingCriteria *giveRemeshingCrit() = 0;

    /**
     * Returns error estimation type of receiver.
     */
    ErrorEstimatorType giveErrorEstimatorType() const { return eeType; }

    /**
     * Returns nonzero if region has been skipped in error estimation (user option).
     * It is strongly recommended to use this function, instead of direct access to
     * regionSkipMap variable by derived classes, since the size check is done here.
     * @param reg Region to check.
     * @return True if region should be skipped.
     */
    bool skipRegion(int reg);
    virtual void reinitialize();

    void initializeFrom(InputRecord &ir) override;
};
} // end namespace oofem
#endif // errorestimator_h
