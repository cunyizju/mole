// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef directerrorindicatorrc_h
#define directerrorindicatorrc_h

#include "utility/interface.h"
#include "mesher/remeshingcrit.h"
#include "math/floatarray.h"
#include "utility/statecountertype.h"

#include <map>

///@name Input fields for DirectErrorIndicatorRC
//@{
#define _IFT_DirectErrorIndicatorRC_minlim "minlim"
#define _IFT_DirectErrorIndicatorRC_maxlim "maxlim"
#define _IFT_DirectErrorIndicatorRC_mindens "mindens"
#define _IFT_DirectErrorIndicatorRC_maxdens "maxdens"
#define _IFT_DirectErrorIndicatorRC_defdens "defdens"
#define _IFT_DirectErrorIndicatorRC_remeshingdensityratio "remeshingdensityratio"
//@}

namespace oofem {
class Domain;
class Element;
class TimeStep;
class ProblemCommunicator;
class ProcessCommunicator;

/**
 * The class is an implementation of "direct" remeshing criteria, which
 * maps the error indication, which is usually the value of observed internal variable
 * to the corresponding required element size. The error estimate and global error for
 * error indicator could not be obtained. The error indication value only tells, that
 * the remeshing is necessary, but no information about the actual error are provided.
 * The task of this class is to simply transform the indicator value to corresponding mesh size.
 */
class DirectErrorIndicatorRC : public RemeshingCriteria
{
protected:
    double minIndicatorLimit, maxIndicatorLimit;
    double minIndicatorDensity, maxIndicatorDensity;
    /// Default mesh density for Indicator value < minIndicatorLimit
    double zeroIndicatorDensity;
    /**
     * Ratio between proposedDensity and currDensity.
     * The remeshing is forced, whenever the actual ratio is smaller than
     * this value. Default value is 0.80
     */
    double remeshingDensityRatioToggle;
    FloatArray nodalDensities;
    /// Actual values (densities) state counter.
    StateCounterType stateCounter;
    RemeshingStrategy currStrategy;
#ifdef __PARALLEL_MODE
    std :: map< int, double >sharedDofManDensities;
    std :: map< int, double >sharedDofManIndicatorVals;
    bool dofManDensityExchangeFlag;
#endif

public:
    /// Constructor
    DirectErrorIndicatorRC(int n, ErrorEstimator * e);
    virtual ~DirectErrorIndicatorRC();

    double giveRequiredDofManDensity(int num, TimeStep *tStep, int relative = 0) override;
    double giveDofManDensity(int num) override;

    void initializeFrom(InputRecord &ir) override;

    int estimateMeshDensities(TimeStep *tStep) override;
    RemeshingStrategy giveRemeshingStrategy(TimeStep *tStep) override;
    /// Returns the minimum indicator limit.
    double giveMinIndicatorLimit() { return minIndicatorLimit; }
    double giveMinIndicatorDensity() { return minIndicatorDensity; }

    void giveNodeChar(int inode, TimeStep *tStep, double &indicatorVal, double &currDensity);
    double giveZeroIndicatorDensity() { return zeroIndicatorDensity; }
    void reinitialize() override;

    void setDomain(Domain *d) override;

    const char *giveInputRecordName() const override { return nullptr; }
    const char *giveClassName() const override { return "DirectErrorIndicatorRC"; }

protected:
    double giveLocalDofManDensity(int num);
    /**
     * Returns dof man indicator values.
     * @param num DofMan number.
     * @param tStep Time step.
     */
    double giveDofManIndicator(int num, TimeStep *tStep);
    double giveLocalDofManIndicator(int num, TimeStep *tStep);
#ifdef __PARALLEL_MODE
    void exchangeDofManDensities();
    int packSharedDofManLocalDensities(ProcessCommunicator &processComm);
    int unpackSharedDofManLocalDensities(ProcessCommunicator &processComm);

    void exchangeDofManIndicatorVals(TimeStep *tStep);
    int packSharedDofManLocalIndicatorVals(ProcessCommunicator &processComm);
    int unpackSharedDofManLocalIndicatorVals(ProcessCommunicator &processComm);
#endif
};
} // end namespace oofem
#endif // directerrorindicatorrc_h
