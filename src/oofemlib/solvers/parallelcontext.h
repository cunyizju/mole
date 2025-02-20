// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef parallelcontext_h
#define parallelcontext_h

#include "oofemcfg.h"
#ifdef __PARALLEL_MODE
 #include "parallel/parallelordering.h"
#endif

namespace oofem {
class EngngModel;
class FloatArray;
class DofManager;

/**
 * This class provides an communication context for distributed memory parallelism.
 * Tasks:
 * - Keeping track of the parallel communicator.
 * - Determining owner for shared dof managers.
 */
class OOFEM_EXPORT ParallelContext
{
protected:
    int di;
    EngngModel *emodel;
#ifdef __PARALLEL_MODE
    Natural2GlobalOrdering n2g;
    Natural2LocalOrdering n2l;
#endif

public:
    /**
     * Creates a context belonging to a system of equations in a given engineering model.
     * @param e Engineering model to work with.
     */
    ParallelContext(EngngModel * e);
    ~ParallelContext();

    /**
     * Initiates the mapping for given domain.
     * @param di Domain index.
     */
    void init(int newDi);

    int giveNumberOfLocalEqs();
    int giveNumberOfGlobalEqs();
    int giveNumberOfNaturalEqs();

    ///@name Convenience functions for working with distributed arrays.
    //@{
    bool isLocal(DofManager *dman);
    /**
     * Norm for a locally distributed array.
     * Common for convergence criterion and such.
     */
    double localNorm(const FloatArray &src);

    /**
     * Dot product for a distributed array.
     * Common for convergence criterion and such.
     */
    double dotProduct(const FloatArray &a, const FloatArray &b);
    /**
     * Dot product for a locally distributed array.
     * Common for convergence criterion and such.
     */
    double localDotProduct(const FloatArray &a, const FloatArray &b);
    /**
     * Accumulates the global value.
     */
    double accumulate(double local);
    /**
     * Accumulates the global value.
     */
    void accumulate(const FloatArray &local, FloatArray &global);
    //@}

#ifdef __PARALLEL_MODE
    Natural2GlobalOrdering *giveN2Gmap() { return & n2g; }
    Natural2LocalOrdering *giveN2Lmap() { return & n2l; }
#endif
};
} // end namespace oofem

#endif // parallelcontext_h
