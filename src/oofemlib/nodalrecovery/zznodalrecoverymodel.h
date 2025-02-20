// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef zznodalrecoverymodel_h
#define zznodalrecoverymodel_h

#include "nodalrecovery/nodalrecoverymodel.h"
#include "utility/interface.h"

#define _IFT_ZZNodalRecoveryModel_Name "zz"

namespace oofem {
class GaussPoint;
class ZZNodalRecoveryModelInterface;
class ProcessCommunicator;

/**
 * The nodal recovery model based on paper of Zienkiewicz and Zhu "A Simple Estimator and Adaptive
 * Procedure for Practical Engineering Analysis". The recovery is based
 * on nodal averaging or projection process in which it is assumed that the stress @f$ \sigma_\star @f$ is
 * interpolated by the same function as the displacement.
 */
class OOFEM_EXPORT ZZNodalRecoveryModel : public NodalRecoveryModel
{
protected:
    /**
     * Helper structure to pass required arguments to packing/unpacking functions
     * needed in parallel mode.
     */
    struct parallelStruct {
        FloatArray *lhs;
        FloatMatrix *rhs;
        IntArray *regionNodalNumbers;
        parallelStruct(FloatArray *a, FloatMatrix *b, IntArray *c) :
            lhs(a), rhs(b), regionNodalNumbers(c) { }
    };

public:
    /// Constructor.
    ZZNodalRecoveryModel(Domain * d);
    /// Destructor.
    virtual ~ZZNodalRecoveryModel();

    int recoverValues(Set elementSet, InternalStateType type, TimeStep *tStep) override;

    const char *giveClassName() const override { return "ZZNodalRecoveryModel"; }

private:
    /**
     * Initializes the region table indicating regions to skip.
     * @param regionMap Region table, the nonzero entry for region indicates region to skip due to
     * unsupported elements or incompatible value size.
     * @param type Determines the type of internal variable to be recovered.
     */
    void initRegionMap(IntArray &regionMap, InternalStateType type);

#ifdef __PARALLEL_MODE
    void initCommMaps();
    void exchangeDofManValues(FloatArray &lhs, FloatMatrix &rhs, IntArray &rn);
    int packSharedDofManData(parallelStruct *s, ProcessCommunicator &processComm);
    int unpackSharedDofManData(parallelStruct *s, ProcessCommunicator &processComm);
#endif
};

/**
 * The element interface required by ZZNodalRecoveryModel.
 */
class OOFEM_EXPORT ZZNodalRecoveryModelInterface : public Interface
{
private:
    Element *element;

public:
    /// Constructor
    ZZNodalRecoveryModelInterface(Element *element): element(element) { }

    /// @name The element interface required by ZZNodalRecoveryModel
    //@{
    /**
     * Computes the element contribution to @f$ \int_\Omega N^{\mathrm{T}}\alpha\;\mathrm{d}\Omega @f$,
     * where @f$ \alpha @f$ is quantity to be recovered (for example stress or strain vector).
     * The size of answer should be recordSize*numberofDofManagers.
     * @param answer Contains the result.
     * @param type Determines the type of internal variable to be recovered.
     * @param tStep Time step.
     * @return true if ok, false if type not supported
     */
    virtual bool ZZNodalRecoveryMI_computeNValProduct(FloatMatrix &answer, InternalStateType type, TimeStep *tStep);
    /**
     * Computes the element contribution to @f$\int_\Omega N^{\mathrm{T}} \cdot N\;\mathrm{d}\Omega @f$ term.
     * The size of answer should be [recordSize*numberofDofManagers].
     * @param answer Contain diagonalized result.
     * @param type Determines the type of internal variable to be recovered.
     */
    virtual void ZZNodalRecoveryMI_computeNNMatrix(FloatArray &answer, InternalStateType type);
    //@}
};
} // end namespace oofem
#endif // zznodalrecoverymodel_h
