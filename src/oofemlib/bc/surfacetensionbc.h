// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef surfacetensionbc_h
#define surfacetensionbc_h

#include "bc/activebc.h"

#include <utility>
#include <list>

///@name Input fields for surface tension boundary condition
//@{
#define _IFT_SurfaceTensionBoundaryCondition_Name "surfacetension"
#define _IFT_SurfaceTensionBoundaryCondition_gamma "gamma"
#define _IFT_SurfaceTensionBoundaryCondition_useTangent "usetangent"
//@}

namespace oofem {
class Element;

/**
 * Computes the load (and possibly tangent) for surface tension. This boundary condition applicable to both solid and flow problems.
 * Computing this load and tangent is involves some fairly complicated expression with lengthy derivations, especially for 3D surfaces.
 */
class OOFEM_EXPORT SurfaceTensionBoundaryCondition : public ActiveBoundaryCondition
{
    double gamma; ///< Surface tension.
    bool useTangent; ///< Determines if tangent should be used.

public:
    /**
     * Constructor. Creates boundary an active condition with given number, belonging to given domain.
     * @param n Boundary condition number.
     * @param d Domain to which new object will belongs.
     */
    SurfaceTensionBoundaryCondition(int n, Domain * d) : ActiveBoundaryCondition(n, d) { }
    /// Destructor.
    virtual ~SurfaceTensionBoundaryCondition() { }

    void initializeFrom(InputRecord &ir) override;

    void assemble(SparseMtrx &answer, TimeStep *tStep,
                  CharType type, const UnknownNumberingScheme &r_s, const UnknownNumberingScheme &c_s, double scale = 1.0, void*lock=nullptr) override;

    void assembleVector(FloatArray &answer, TimeStep *tStep,
                        CharType type, ValueModeType mode,
                        const UnknownNumberingScheme &s, 
                        FloatArray *eNorms=nullptr,
                        void*lock=nullptr) override;

    void giveLocationArrays(std :: vector< IntArray > &rows, std :: vector< IntArray > &cols, CharType type,
                            const UnknownNumberingScheme &r_s, const UnknownNumberingScheme &c_s) override;

    const char *giveClassName() const override { return "SurfaceTensionBoundaryCondition"; }
    const char *giveInputRecordName() const override { return _IFT_SurfaceTensionBoundaryCondition_Name; }

protected:
    /**
     * Helper function for computing the contributions to the load vector.
     */
    void computeLoadVectorFromElement(FloatArray &answer, Element *e, int side, TimeStep *tStep);
    /**
     * Helper function for computing the tangent (@f$ K = \frac{\mathrm{d}F}{\mathrm{d}u} @f$)
     */
    void computeTangentFromElement(FloatMatrix &answer, Element *e, int side, TimeStep *tStep);
};
} // end namespace oofem
#endif // surfacetensionbc_h
