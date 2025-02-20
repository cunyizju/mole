// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef remeshingcrit_h
#define remeshingcrit_h

#include "input/femcmpnn.h"
#include "utility/interface.h"

namespace oofem {
class Domain;
class Element;
class TimeStep;
class ErrorEstimator;
class CommunicatorBuff;
class ProblemCommunicator;

/// Type representing the remeshing strategy
enum RemeshingStrategy { NoRemeshing_RS, RemeshingFromCurrentState_RS, RemeshingFromPreviousState_RS };

/**
 * The base class for all remeshing criteria.
 * The basic task is to evaluate the required mesh density (at nodes) on given domain,
 * based on informations provided by the compatible error estimator.
 * If this task requires the special element algorithms, these should be included using interface concept.
 *
 * The remeshing criteria is maintained by the corresponding error estimator. This is mainly due to fact, that is
 * necessary for given EE to create compatible RC. In our concept, the EE is responsible.
 */
class OOFEM_EXPORT RemeshingCriteria : public FEMComponent
{
protected:
    ErrorEstimator *ee;

#ifdef __PARALLEL_MODE
    /// Common Communicator buffer.
    CommunicatorBuff *commBuff;
    /// Communicator.
    ProblemCommunicator *communicator;
    /// Communication init flag.
    bool initCommMap;
#endif

public:
    /// Constructor
    RemeshingCriteria(int n, ErrorEstimator * e);
    /// Destructor
    virtual ~RemeshingCriteria();
    /**
     * Returns the required mesh size n given dof manager.
     * The mesh density is defined as a required element size
     * (in 1D the element length, in 2D the square from element area).
     * @param num Dofman number.
     * @param tStep Time step.
     * @param relative If zero, then actual density is returned, otherwise the relative density to current is returned.
     */
    virtual double giveRequiredDofManDensity(int num, TimeStep *tStep, int relative = 0) = 0;
    /**
     * Returns existing mesh size for given dof manager.
     * @param num DofMan number.
     */
    virtual double giveDofManDensity(int num) = 0;

    /**
     * Determines, if the remeshing is needed, and if needed, the type of strategy used.
     * @param tStep Time step.
     */
    virtual RemeshingStrategy giveRemeshingStrategy(TimeStep *tStep) = 0;
    /**
     * Estimates the nodal densities.
     * @param tStep Time step.
     */
    virtual int estimateMeshDensities(TimeStep *tStep) = 0;

    virtual void reinitialize() { }
};
} // end namespace oofem
#endif // remeshingcrit_h
