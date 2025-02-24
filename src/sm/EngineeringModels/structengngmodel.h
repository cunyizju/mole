// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef structengngmodel_h
#define structengngmodel_h

#include "engng/engngm.h"
#include "utility/statecountertype.h"
#include "math/floatarray.h"

namespace oofem {
class StructuralElement;

/// Assembles the internal forces, without updating the strain.
///@todo The need for this is just due to some other design choices. 
class LastEquilibratedInternalForceAssembler : public InternalForceAssembler
{
    void vectorFromElement(FloatArray &vec, Element &element, TimeStep *tStep, ValueModeType mode) const override;
};

/**
 * Callback class for assembling linearized thermal "loads", useful for computing initial guesses.
 * @author Mikael Öhman
 */
class LinearizedDilationForceAssembler : public VectorAssembler
{
public:
    void vectorFromElement(FloatArray &vec, Element &element, TimeStep *tStep, ValueModeType mode) const override;
};

/**
 * Callback class for assembling initial stress matrices
 * @author Mikael Öhman
 */
class InitialStressMatrixAssembler : public MatrixAssembler
{
public:
    void matrixFromElement(FloatMatrix &mat, Element &element, TimeStep *tStep) const override;
};

/**
 * Callback class for assembling lumped initial stress matrices
 * @author Mikael Öhman
 */
class LumpedInitialStressMatrixAssembler : public MatrixAssembler
{
public:
    void matrixFromElement(FloatMatrix &mat, Element &element, TimeStep *tStep) const override;
};

/**
 * This class implements extension of EngngModel for structural models.
 * Its purpose is to declare and implement general methods for computing reaction forces.
 */
class StructuralEngngModel : public EngngModel
{
protected:
    /**
     * Contains last time stamp of internal variable update.
     * This update is made via various services
     * (like those for computing real internal forces or updating the internal state).
     */
    StateCounterType internalVarUpdateStamp;

    /// Norm of nodal internal forces evaluated on element by element basis (squared)
    FloatArray internalForcesEBENorm;
    /**
     * Computes and prints reaction forces, computed from nodal internal forces. Assumes, that real
     * stresses corresponding to reached state are already computed (uses giveInternalForcesVector
     * structural element service with useUpdatedGpRecord = 1 parameter). Only the dof managers selected for
     * output (OutputManager) are handled.
     * @see StructuralElement::giveInternalForcesVector
     * @see OutputManager
     * @param tStep Time step.
     * @param id Domain number.
     */
    void printReactionForces(TimeStep *tStep, int id, FILE *out);

    /**
     * Computes the contribution external loading to reaction forces in given domain. Default implementations adds the
     * contribution from computeElementLoadReactionContribution and computeElementLoadReactionContribution methods.
     * @param reactions Contains the computed contributions.
     * @param tStep Solution step.
     * @param di Domain number.
     */
    virtual void computeExternalLoadReactionContribution(FloatArray &reactions, TimeStep *tStep, int di);

    /**
     * Updates nodal values
     * (calls also this->updateDofUnknownsDictionary for updating dofs unknowns dictionaries
     * if model supports changes of static system). The element internal state update is also forced using
     * updateInternalState service.
     * @param tStep Solution step.
     */
    void updateInternalState(TimeStep *tStep);

    void printOutputAt(FILE *file, TimeStep *tStep) override;

public:
    /// Creates new StructuralEngngModel with number i, associated to domain d.
    StructuralEngngModel(int i, EngngModel *master = nullptr);
    /// Destructor.
    virtual ~StructuralEngngModel();

    void updateYourself(TimeStep *tStep) override;

    int checkConsistency() override;

    /**
     * Computes reaction forces. The implementation assumes, that real
     * stresses corresponding to reached state are already computed (uses giveInternalForcesVector
     * structural element service with useUpdatedGpRecord = 1 parameter).
     * To be safe, this method should be called after convergence has been reached, eq.,
     * after engngModel->updateYourself() has been called.
     * @param answer Reactions, the ordering of individual values follows numbering of prescribed equations.
     * @param tStep Time step.
     * @param di Domain number.
     */
    void computeReaction(FloatArray &answer, TimeStep *tStep, int di);

    void terminate(TimeStep *tStep) override;

    /**
     * Builds the reaction force table. For each prescribed equation number it will find
     * corresponding node and dof number. The entries in the restrDofMans, restrDofs, and eqn
     * arrays are sorted with increasing dofman number and with increasing dof number as
     * a second minor criterion.
     * @param restrDofMans Contains numbers of restrained Dofmanagers, with size equal to total number of prescribed equations.
     * @param restrDofs Contains IDs of restrained Dofs, with size equal to total number of prescribed equations.
     * @param eqn Contains the corresponding restrained equation numbers.
     * @param tStep Time step.
     * @param di Domain number.
     */
    void buildReactionTable(IntArray &restrDofMans, IntArray &restrDofs, IntArray &eqn, TimeStep *tStep, int di);

    void updateInternalRHS(FloatArray &answer, TimeStep *tStep, Domain *d, FloatArray *eNorm) override;
};
} // end namespace oofem
#endif // structengngmodel_h
