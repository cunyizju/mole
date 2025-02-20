// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef parallelordering_h
#define parallelordering_h

#include "oofemcfg.h"
#include "math/intarray.h"
#include "dofman/dofmanager.h"

#include <map>

namespace oofem {
class EngngModel;
class IntArray;
class UnknownNumberingScheme;

class OOFEM_EXPORT ParallelOrdering
{
public:
    ParallelOrdering() { }
    virtual ~ParallelOrdering() { }

    /// Returns true if given DofManager is local (ie maintained by the receiver processor).
    bool isLocal(DofManager *dman);
    /// Returns true if given DofManager is shared between partitions.
    bool isShared(DofManager *dman);

    /**
     * Initiates the receiver.
     * @param em Engineering model to determine general information about the problem.
     * @param di Domain index.
     */
    virtual void init(EngngModel *em, int di, const UnknownNumberingScheme &u) = 0;

    /**
     * Returns number of local eqs; ie. those that belong to receiver processor;
     * Note that some eqs may be owned by remote processors (some shared nodes,...).
     * The sum of local eqs for all processors should give total number of eqs.
     * @return Numbering of local equations.
     */
    virtual int giveNumberOfLocalEqs() { return 0; }
    /**
     * @return the total number of equations of the problem.
     */
    virtual int giveNumberOfGlobalEqs() { return 0; }

    /**
     * Finds the global equation from a local equation.
     * @param leq Local equation number.
     * @return Global equation number.
     */
    virtual int giveNewEq(int leq) = 0;
    /**
     * Finds the local equation number from a global equation.
     * @param eq Global equation number.
     * @return Local equation number.
     */
    virtual int giveOldEq(int eq) = 0;

    virtual void map2New(IntArray &answer, const IntArray &src, int baseOffset = 0) = 0;
    virtual void map2Old(IntArray &answer, const IntArray &src, int baseOffset = 0) = 0;
};

/**
 * Ordering from oofem natural ordering (includes all local and shared eqs)
 * to global ordering.
 */
class OOFEM_EXPORT Natural2GlobalOrdering : public ParallelOrdering
{
protected:
    /// Old to new mapping; uses 0-based global eq ordering; 1-based local ordering.
    IntArray locGlobMap;
    /// New to old mapping.
    std :: map< int, int >globLocMap;

    /// Number of local and global eqs.
    int l_neqs, g_neqs;

public:
    Natural2GlobalOrdering();
    virtual ~Natural2GlobalOrdering() { }

    void init(EngngModel *, int di, const UnknownNumberingScheme &n) override;

    int giveNewEq(int leq) override;
    int giveOldEq(int eq) override;

    void map2New(IntArray &answer, const IntArray &src, int baseOffset = 0) override;
    void map2Old(IntArray &answer, const IntArray &src, int baseOffset = 0) override;

    int giveNumberOfLocalEqs() override { return l_neqs; }
    int giveNumberOfGlobalEqs() override { return g_neqs; }

    IntArray *giveN2Gmap() { return & locGlobMap; }
};

/**
 * Ordering from oofem natural ordering (includes all local and shared eqs)
 * to local ordering, where only locally maintained eqs are considered.
 */
class OOFEM_EXPORT Natural2LocalOrdering : public ParallelOrdering
{
protected:
    /// Natural to local
    IntArray n2l;

public:
    Natural2LocalOrdering();
    virtual ~Natural2LocalOrdering() { }

    void init(EngngModel *, int di, const UnknownNumberingScheme &n) override;

    int giveNewEq(int leq) override;
    int giveOldEq(int eq) override;

    void map2New(IntArray &answer, const IntArray &src, int baseOffset = 0) override;
    void map2Old(IntArray &answer, const IntArray &src, int baseOffset = 0) override;

    IntArray *giveN2Lmap() { return & n2l; }
};
} // end namespace oofem

#endif // parallelordering_h
