// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef mixedgradientpressurecneumann_h
#define mixedgradientpressurecneumann_h

#include "bc/mixedgradientpressurebc.h"
#include "bc/boundarycondition.h"
#include "dofman/dof.h"
#include "bc/bctype.h"
#include "input/valuemodetype.h"
#include "math/floatarray.h"
#include "math/floatmatrix.h"

#include <memory>

#define _IFT_MixedGradientPressureNeumann_Name   "mixedgradientpressureneumann"

namespace oofem {
class MasterDof;
class Node;
class IntegrationRule;
class SparseMtrx;
class SparseLinearSystemNM;
class Element;

/**
 * Applies a mean deviatoric shear rate and pressure (Neumann boundary condition).
 * Introduces unknowns for the deviatoric stress, and this boundary condition adds contributions to both the left and right hand side of the system.
 * The approach expresses the deviatoric stresses/strain(rates) in a special (nonsymmetric) base;
 * @f[
 * \boldsymbol A_{\mathrm{dev}} = \sum_{i=1}^{n} A_{\mathrm{dev},i} \boldsymbol E_i
 * @f]
 * where the bases @f$ \boldsymbol E_i @f$ can be chosen as
 * @f[
 * \boldsymbol E_1 = \frac{1}{\sqrt{2}}\begin{pmatrix}1 & 0 \\ 0 & -1\end{pmatrix}, \quad
 * \boldsymbol E_2 = \begin{pmatrix}0 & 1 \\ 0 & 0\end{pmatrix}, \quad
 * \boldsymbol E_3 = \begin{pmatrix}0 & 0 \\ 1 & 0\end{pmatrix}.
 * @f]
 * in 2D, and
 * @f[
 * \boldsymbol E_1 = \frac{1}{\sqrt{6}}\begin{pmatrix}2 & 0 & 0\\ 0 & -1 & 0 \\ 0 & 0 & -1\end{pmatrix}, \quad
 * \boldsymbol E_2 = \frac{1}{\sqrt{2}}\begin{pmatrix}0 & 0 & 0\\ 0 &  1 & 0 \\ 0 & 0 & -1\end{pmatrix}, \quad
 * \boldsymbol E_3 = \begin{pmatrix}0 & 1 & 0\\ 0 & 0 & 0 \\ 0 & 0 & 0\end{pmatrix}, \ldots\quad
 * \boldsymbol E_8 = \begin{pmatrix}0 & 0 & 0\\ 0 & 0 & 0 \\ 0 & 1 & 0\end{pmatrix}
 * @f]
 * in 3D (in 1D no deviatoric component exists).
 *
 * Either bulk elements + side number can be used, or a boundary element (line in 2D, surface in 3D) can be used directly.
 *
 * @note The 2D case assumes plane strain(rate), as is the case in _2dFlow.
 * @note This is only applicable to momentum balance equation. Both solid or fluids, incompressible or compressible, should work.
 * @note Should typically be prescribed on the entire external boundary of an representative volume element.
 * @note Should be applied to element boundaries, not DOFs.
 * @note The implementation doesn't assume that the stress is symmetric, so rigid body rotations are automatically removed.
 * @note Rigid body translations must be controlled separately.
 *
 * @author Mikael Öhman
 */
class OOFEM_EXPORT MixedGradientPressureNeumann : public MixedGradientPressureBC
{
protected:
    /// Prescribed gradient @f$ d_{\mathrm{dev},ij} @f$ in Voigt form.
    FloatArray devGradient;
    /**
     * The volumetric part of what was sent in (needed to return the difference).
     * If caller takes care and sends in a deviatoric gradient, then this will be zero and the return value for the volumetric part will be the true volumetric change.
     */
    double volGradient;

    /// Prescribed pressure.
    double pressure;

    /// DOF-manager containing the unknown deviatoric stress.
    std :: unique_ptr< Node > sigmaDev;
    /// Dof IDs for the lagrange multipliers in sigmaDev
    IntArray dev_id;

public:
    /**
     * Creates boundary condition with given number, belonging to given domain.
     * @param n Boundary condition number.
     * @param d Domain to which new object will belongs.
     */
    MixedGradientPressureNeumann(int n, Domain * d);

    /// Destructor
    virtual ~MixedGradientPressureNeumann();

    /**
     * Returns the number of internal DOF managers (=2).
     * This boundary condition stores its own DOF managers, one for @f$ d_{\mathrm{dev},ij} @f$ in which the DOFs are prescribed
     * and one for @f$ d_{\mathrm{vol}} @f$ for single free volumetric strain rate.
     */
    int giveNumberOfInternalDofManagers() override;
    /**
     * Returns the volumetric DOF manager for i == 1, and the deviatoric manager for i == 2.
     */
    DofManager *giveInternalDofManager(int i) override;

    /// Not relevant for this boundary condition.
    bcType giveType() const override { return UnknownBT; }

    /**
     * Initializes receiver according to object description stored in input record.
     * The input record contains two fields;
     * - devGradient \#columns { d_11 d_22 ... d_21 ... } (required)
     * - pressure p (required)
     * The gradient should be in Voigt notation (only the deviatoric part will be used)
     */
    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    void scale(double s) override;

    void computeFields(FloatArray &sigmaDev, double &vol, TimeStep *tStep) override;
    void computeTangents(FloatMatrix &Ed, FloatArray &Ep, FloatArray &Cd, double &Cp, TimeStep *tStep) override;

    void setPrescribedPressure(double p) override { pressure = p; }
    void setPrescribedDeviatoricGradientFromVoigt(const FloatArray &ddev) override;

    void assembleVector(FloatArray &answer, TimeStep *tStep,
                        CharType type, ValueModeType mode,
                        const UnknownNumberingScheme &s, FloatArray *eNorm=nullptr,
                        void*lock=nullptr) override;

    void assemble(SparseMtrx &answer, TimeStep *tStep,
                  CharType type, const UnknownNumberingScheme &r_s, const UnknownNumberingScheme &c_s, 
                  double scale = 1.0, void*lock=nullptr) override;

    void giveLocationArrays(std :: vector< IntArray > &rows, std :: vector< IntArray > &cols, CharType type,
                            const UnknownNumberingScheme &r_s, const UnknownNumberingScheme &c_s) override;

    const char *giveClassName() const override { return "MixedGradientPressureNeumann"; }
    const char *giveInputRecordName() const override { return _IFT_MixedGradientPressureNeumann_Name; }

protected:
    /// Helper function that integrates the deviatoric tangent contribution from a single element boundary.
    void integrateDevTangent(FloatMatrix &answer, Element *e, int boundary);
    /// Helper function that integrates the volumetric tangent contribution from a single element boundary.
    void integrateVolTangent(FloatArray &answer, Element *e, int boundary);

    /// Converts from deviatoric to (normal) cartesian base (arrays are second order 2D tensors in Voigt notation)
    void fromDeviatoricBase2D(FloatArray &cartesian, FloatArray &deviatoric);
    /// Converts from deviatoric to (normal) cartesian base (arrays are second order 3D tensors in Voigt notation)
    void fromDeviatoricBase3D(FloatArray &cartesian, FloatArray &deviatoric);
    /// Converts from deviatoric to (normal) cartesian base (arrays are fourth order 2D tensors in Voigt notation)
    void fromDeviatoricBase2D(FloatMatrix &cartesian, FloatMatrix &deviatoric);
    /// Converts from deviatoric to (normal) cartesian base (arrays are fourth order 3D tensors in Voigt notation)
    void fromDeviatoricBase3D(FloatMatrix &cartesian, FloatMatrix &deviatoric);
};
} // end namespace oofem

#endif // mixedgradientpressurecneumann_h
