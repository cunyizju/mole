// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef nldeidynamic_h
#define nldeidynamic_h

#include "sm/EngineeringModels/structengngmodel.h"
#include "math/floatarray.h"
#include "math/floatmatrix.h"
#include "solvers/sparselinsystemnm.h"
#include "math/sparsemtrxtype.h"

#include <memory>

#define LOCAL_ZERO_MASS_REPLACEMENT 1

///@name Input fields for NlDEIDynamic
//@{
#define _IFT_NlDEIDynamic_Name "nldeidynamic"
#define _IFT_NlDEIDynamic_dumpcoef "dumpcoef"
#define _IFT_NlDEIDynamic_deltat "deltat"
#define _IFT_NlDEIDynamic_drflag "drflag"
#define _IFT_NlDEIDynamic_tau "tau"
#define _IFT_NlDEIDynamic_py "py"
#define _IFT_NlDEIDynamic_nonlocalext "nonlocalext"
#define _IFT_NlDEIDynamic_reduct "reduct"
//@}

namespace oofem {
/**
 * This class implements NonLinear (- may be changed) solution of dynamic
 * problems using Direct Explicit Integration scheme - Central Difference
 * Method. For efficiency reasons it uses diagonal mass matrix. It is formulated
 * in increments of displacements rather than in total variables.
 *
 * Solution of this problem is series of loading cases, maintained as sequence of
 * time-steps.
 *
 * Analysis starts assembling governing equations at time step 0 ( 0 given by boundary and initial cond.)
 * they result in response at time step 1. For time step 0 we need special start code.
 * Because this method is explicit, when solving equations for step t, we obtain
 * solution in step t+dt. But printing is performed for step t.
 * So, when analyst specifies initial conditions, then he/she specifies them in time step 0.
 *
 * Current implementation supports parallel processing. Both node- and element cut strategies can
 * be used.
 * - In node cut strategy, partitions are divided using cut, which goes through nodes.
 *   These cut nodes are called "shared" nodes. Generally, unknown values in shared nodes are
 *   composed from local partition contributions as well as from contributions from remote partitions
 *   sharing this node. Particularly, masses and real nodal forces have to be exchanged for shared
 *   nodes.
 * - In element cut strategy, partitions are divided using cut running through elements. The cut elements are
 *   replicated on neighbouring partitions. The nodes belonging to  replicated elements belonging to
 *   remote partitions are called remote nodes. The are mirrors or remote copies of corresponding
 *   nodes on neighbouring partition.
 * - Additional mode has been introduced remote element mode. It introduces the "remote" elements, the
 *   exact local mirrors of remote counterparts. Introduced to support general nonlocal constitutive models,
 *   in order to provide efficient way, how to average local data without need of fine grain communication.
 */
class NlDEIDynamic : public StructuralEngngModel
{
protected:
    /// Mass matrix.
    FloatArray massMatrix;
    /// Load vector.
    FloatArray loadVector;
    /// Vector storing  displacement increments.
    FloatArray previousIncrementOfDisplacementVector;
    /// Displacement, velocity and acceleration vectors.
    FloatArray displacementVector, velocityVector, accelerationVector;
    /// Vector of real nodal forces.
    FloatArray internalForces;
    /// Dumping coefficient (C = dumpingCoef * MassMtrx).
    double dumpingCoef;
    /// Time step.
    double deltaT;
    /// Flag indicating the need for initialization.
    int initFlag;
    /// Optional reduction factor for time step deltaT
    double reductionFactor;
    // dynamic relaxation specific vars
    /// Flag indicating whether dynamic relaxation takes place.
    int drFlag;
    /// Reference load vector.
    FloatArray loadRefVector;
    /// Parameter determining rate of the loading process.
    double c;
    /// Load level.
    double pt;
    /// End of time interval.
    double Tau;
    /// Estimate of loadRefVector^T*displacementVector(Tau).
    double pyEstimate;
    /// Product of p^tM^(-1)p; where p is reference load vector.
    double pMp;

    LinSystSolverType solverType;
    SparseMtrxType sparseMtrxType;
    std::unique_ptr<SparseLinearSystemNM> nMethod;

public:
    NlDEIDynamic(int i, EngngModel *master = nullptr);

    virtual ~NlDEIDynamic();

    void solveYourself() override;
    void solveYourselfAt(TimeStep *tStep) override;

    void updateYourself(TimeStep *tStep) override;
    double giveUnknownComponent(ValueModeType type, TimeStep *tStep, Domain *d, Dof *dof) override;
    void initializeFrom(InputRecord &ir) override;

    TimeStep *giveNextStep() override;
    NumericalMethod *giveNumericalMethod(MetaStep *mStep) override;

    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;

    void printOutputAt(FILE *file, TimeStep *tStep) override;

    void printDofOutputAt(FILE *stream, Dof *iDof, TimeStep *tStep) override;

    // identification
    const char *giveInputRecordName() const { return _IFT_NlDEIDynamic_Name; }
    const char *giveClassName() const override { return "NlDEIDynamic"; }
    fMode giveFormulation() override { return TL; }

    int giveNumberOfFirstStep(bool force = false) override { return 0; }

protected:
    /**
     * Assembles the load vector.
     * If in parallel mode, the loads of shared/remote nodes are exchanged and remote contributions are taken into account.
     * @param answer Load vector.
     * @param mode Value type mode of load vector.
     * @param tStep Solution step.
     */
    void computeLoadVector(FloatArray &answer, ValueModeType mode, TimeStep *tStep);
    /**
     * Assembles the diagonal mass matrix of receiver.
     * Local or Global variant of zero mass elements replacement is performed.
     * If runs in parallel, the masses of shared nodes are exchanged and
     * remote contributions are added accordingly.
     * @param mass Assembled mass matrix.
     * @param maxOm Estimate of eigenfrequency.
     * @param tStep Time step.
     */
    void computeMassMtrx(FloatArray &mass, double &maxOm, TimeStep *tStep);
    void computeMassMtrx2(FloatMatrix &mass, double &maxOm, TimeStep *tStep);

public:
    int estimateMaxPackSize(IntArray &commMap, DataStream &buff, int packUnpackType) override;
};
} // end namespace oofem
#endif // nldeidynamic_h
