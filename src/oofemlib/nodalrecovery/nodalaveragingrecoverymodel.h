// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef nodalaveragingrecoverymodel_h
#define nodalaveragingrecoverymodel_h

#include "nodalrecovery/nodalrecoverymodel.h"
#include "utility/interface.h"

#define _IFT_NodalAveragingRecoveryModel_Name "nodalaverage"

namespace oofem {
class GaussPoint;
class ProcessCommunicator;

/**
 * The nodal recovery model based on nodal averaging. The recovery is based
 * on nodal averaging or projection process in which element contributions are averaged
 * at node with the same weight.
 */
class OOFEM_EXPORT NodalAveragingRecoveryModel : public NodalRecoveryModel
{
protected:
    /**
     * Helper structure to pass required arguments to packing/unpacking functions
     * needed in parallel mode
     */
    struct parallelStruct {
        FloatArray *lhs;
        IntArray *regionDofMansConnectivity;
        IntArray *regionNodalNumbers;
        int regionValSize;
        parallelStruct(FloatArray *a, IntArray *b, IntArray *c, int d) :
            lhs(a), regionDofMansConnectivity(b), regionNodalNumbers(c), regionValSize(d) { }
    };

public:
    /// Constructor.
    NodalAveragingRecoveryModel(Domain * d);
    /// Destructor.
    virtual ~NodalAveragingRecoveryModel();

    int recoverValues(Set elementSet, InternalStateType type, TimeStep *tStep) override;

    const char *giveClassName() const override { return "NodalAveragingRecoveryModel"; }

private:
#ifdef __PARALLEL_MODE
    void initCommMaps();
    void exchangeDofManValues(FloatArray &lhs, IntArray &, IntArray &, int);
    int packSharedDofManData(parallelStruct *s, ProcessCommunicator &processComm);
    int unpackSharedDofManData(parallelStruct *s, ProcessCommunicator &processComm);
#endif
};

/**
 * The element interface required by NodalAvergagingRecoveryModel.
 */
class OOFEM_EXPORT NodalAveragingRecoveryModelInterface : public Interface
{
public:
    /// Constructor
    NodalAveragingRecoveryModelInterface() { }

    /// @name The element interface required by NodalAveragingRecoveryModel
    //@{
    /**
     * Computes the element value in given node.
     * @param answer Contains the result.
     * @param node Element node number.
     * @param type Determines the type of internal variable to be recovered.
     * @param tStep Time step.
     */
    virtual void NodalAveragingRecoveryMI_computeNodalValue(FloatArray &answer, int node,
                                                            InternalStateType type, TimeStep *tStep) = 0;
    //@}
};
} // end namespace oofem
#endif // nodalaveragingrecoverymodel_h
