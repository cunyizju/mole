// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef deidynamic_h
#define deidynamic_h

#include "sm/EngineeringModels/structengngmodel.h"

///@name Input fields for DEIDynamic
//@{
#define _IFT_DEIDynamic_Name "deidynamic"
#define _IFT_DEIDynamic_dumpcoef "dumpcoef"
#define _IFT_DEIDynamic_deltat "deltat"
//@}

namespace oofem {
/**
 * This class implements Linear (- may be changed) solution of dynamic
 * problems using Direct Explicit Integration scheme - Central Difference
 * Method. For efficiency reasons it uses diagonal mass matrix
 *
 * Description:
 * Solution of this problem is series of loading cases, maintained as sequence of
 * time-steps. For obtaining diagonal mass matrix from possibly non-diagonal one
 * returned from Element::giveMassMatrix() a FloatMatrix::Lumped() is called
 * to obtain diagonal form.
 *
 * We start assemble governing equations at time step 0 ( 0 given by boundary and initial cond.)
 * they result in response at time step 1.
 * for time step 0 we need special start code.
 * so we obtain solution for time step 1 and next.
 * because this method is explicit, when solving equations for step t, we obtain
 * solution in step t+dt. But printing is performed for step t.
 * see diidynamic.h for difference.
 * So, when You specify initial conditions, you specify them in time step 0.
 *
 * Tasks:
 * - Creating Numerical method for solving Ax=b
 * - Interfacing Numerical method to Elements
 * - Managing time steps
 */
class DEIDynamic : public StructuralEngngModel
{
protected:
    FloatArray massMatrix;
    FloatArray loadVector;
    FloatArray nextDisplacementVector;
    FloatArray displacementVector, velocityVector, accelerationVector;
    double dumpingCoef, deltaT;

public:
    DEIDynamic(int i, EngngModel *master = nullptr) : StructuralEngngModel(i, master), massMatrix(), loadVector(),
        nextDisplacementVector(), displacementVector(), velocityVector(), accelerationVector() {
        ndomains = 1;
    }
    virtual ~DEIDynamic();

    void solveYourselfAt(TimeStep *tStep) override;

    double giveUnknownComponent(ValueModeType type, TimeStep *tStep, Domain *d, Dof *dof) override;
    void initializeFrom(InputRecord &ir) override;
    TimeStep *giveNextStep() override;
    NumericalMethod *giveNumericalMethod(MetaStep *mStep) override;

    void printDofOutputAt(FILE *stream, Dof *iDof, TimeStep *tStep) override;

    // identification
    const char *giveClassName() const override { return "DEIDynamic"; }
    const char *giveInputRecordName() const { return _IFT_DEIDynamic_Name; }
    fMode giveFormulation() override { return TL; }
    int giveNumberOfFirstStep(bool force = false) override { return 0; }
};
} // end namespace oofem
#endif // deidynamic_h
