// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef prescribedgradient_h
#define prescribedgradient_h

#include "bc/prescribedgradienthomogenization.h"
#include "bc/boundarycondition.h"
#include "dofman/dof.h"
#include "bc/bctype.h"
#include "input/valuemodetype.h"
#include "math/floatarray.h"
#include "math/floatmatrix.h"

///@name Input fields for PrescribedGradient
//@{
#define _IFT_PrescribedGradient_Name "prescribedgradient"
//@}

namespace oofem {
/**
 * Prescribes @f$ v_i = d_{ij}(x_j-\bar{x}_j) @f$ or @f$ s = d_{1j}(x_j - \bar{x}_j) @f$
 * where @f$ v_i @f$ are primary unknowns for the subscale.
 * This is typical boundary condition in multiscale analysis where @f$ d = \partial_x s@f$
 * would a macroscopic gradient at the integration point, i.e. this is a boundary condition for prolongation.
 * It is also convenient to use when one wants to test a arbitrary specimen for shear.
 * @author Mikael Öhman
 * @author Erik Svenning
 */
class OOFEM_EXPORT PrescribedGradient : public BoundaryCondition, public PrescribedGradientHomogenization
{
public:
    /**
     * Creates boundary condition with given number, belonging to given domain.
     * @param n Boundary condition number.
     * @param d Domain to which new object will belongs.
     */
    PrescribedGradient(int n, Domain *d) : BoundaryCondition(n, d) { }

    /// Destructor
    virtual ~PrescribedGradient() { }

    double give(Dof *dof, ValueModeType mode, double time) override;

    bcType giveType() const override { return DirichletBT; }

    /**
     * Initializes receiver according to object description stored in input record.
     * The input record contains two fields;
     * - gradient \#rows \#columns { d_11 d_12 ... ; d_21 ... } (required)
     * - cCoords \#columns x_1 x_2 ... (optional, default 0)
     * The prescribed tensor's columns must be equal to the size of the center coordinates.
     * The size of the center coordinates must be equal to the size of the coordinates in the applied nodes.
     */
    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    /**
     * Constructs a coefficient matrix for all prescribed unknowns.
     * Helper routine for computational homogenization.
     * @todo Perhaps this routine should only give results for the dof it prescribes?
     * @param C Coefficient matrix to fill.
     */
    virtual void updateCoefficientMatrix(FloatMatrix &C);

    /**
     * Computes the homogenized, macroscopic, field (stress).
     * @param sigma Output quantity (typically stress).
     * @param tStep Active time step.
     */
    void computeField(FloatArray &sigma, TimeStep *tStep) override;

    /**
     * Computes the macroscopic tangent for homogenization problems through sensitivity analysis.
     * @param tangent Output tangent.
     * @param tStep Active time step.
     */
    void computeTangent(FloatMatrix &tangent, TimeStep *tStep) override;

    void scale(double s) override { mGradient.times(s); }

    const char *giveClassName() const override { return "PrescribedGradient"; }
    const char *giveInputRecordName() const override { return _IFT_PrescribedGradient_Name; }
};
} // end namespace oofem

#endif // prescribedgradient_h
