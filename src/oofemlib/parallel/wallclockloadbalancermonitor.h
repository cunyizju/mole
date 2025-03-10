// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef wallclockloadbalancer_h
#define wallclockloadbalancer_h

#include "input/loadbalancer.h"

#define __LB_DEBUG
#ifdef __LB_DEBUG
 #include <list>
 #include "utility/range.h"
 #include "math/intarray.h"
 #include "math/floatarray.h"
#endif

///@name Input fields for WallClockLoadBalancerMonitor
//@{
#define _IFT_WallClockLoadBalancerMonitor_Name "wallclock"
#define _IFT_WallClockLoadBalancerMonitor_relwct "relwct"
#define _IFT_WallClockLoadBalancerMonitor_abswct "abswct"
#define _IFT_WallClockLoadBalancerMonitor_minwct "minwct"
#define _IFT_WallClockLoadBalancerMonitor_lbstep "lbstep"
#define _IFT_WallClockLoadBalancerMonitor_perturbedsteps "lbperturbedsteps"
#define _IFT_WallClockLoadBalancerMonitor_perturbfactor "lbperturbfactor"
#define _IFT_WallClockLoadBalancerMonitor_recoveredsteps "lbrecoveredsteps"
#define _IFT_WallClockLoadBalancerMonitor_processingweights "lbprocessingweights"
//@}

namespace oofem {

/**
 * Implementation of simple wall-clock based monitor.
 * It detect imbalance based on wall clock difference required for solution step
 * on particular nodes. When difference in wall clock solution times is greater
 * than a threshold value, the load migration is performed.
 */
class OOFEM_EXPORT WallClockLoadBalancerMonitor : public LoadBalancerMonitor
{
protected:
    /// Declares min abs imbalance to perform relative imbalance check.
    double relWallClockImbalanceTreshold, absWallClockImbalanceTreshold, minAbsWallClockImbalanceTreshold;
    /// The rebalancing done every lbstep.
    int lbstep;
 #ifdef __LB_DEBUG
    /// List of steps with perturbed balancing.
    std :: list< Range >perturbedSteps;
    /// Perturbing factor.
    double perturbFactor;
    /// list of step at which to performed lb recovery.
    IntArray recoveredSteps;
    /// processing weights for lb recovery.
    FloatArray processingWeights;
 #endif
public:
    WallClockLoadBalancerMonitor(EngngModel * em) :
        LoadBalancerMonitor(em),
        relWallClockImbalanceTreshold(0.1),
        absWallClockImbalanceTreshold(10.0),
        minAbsWallClockImbalanceTreshold(0.0),
        lbstep(5)
    { }

    LoadBalancerDecisionType decide(TimeStep *) override;

    void initializeFrom(InputRecord &ir) override;

    const char *giveClassName() const override { return "WallClockLoadBalancerMonitor"; }
};

} // end namespace oofem
#endif //wallclockloadbalancer_h
