// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef variablecrosssection_h
#define variablecrosssection_h

#include "sm/CrossSections/simplecrosssection.h"
#include "sm/CrossSections/layeredcrosssection.h"
#include "sm/Materials/Structural/structuralmaterial.h"
#include "math/floatarray.h"
#include "math/floatmatrix.h"
#include "func/scalarfunction.h"

///@name Input fields for SimpleCrossSection
//@{
#define _IFT_VariableCrossSection_Name "variablecs"
//@}

namespace oofem {
/**
 * Class implementing cross section model in finite element problem.
 * A cross section properties are not a constant values, but they can be set as
 * scalar functions of the spatial position in terms of x,y,z variables.
 * example "0.1+x*0.2+y*0.0", where (x,y,z) are global (or optionally local element) coordinates.
 * This cross section model is a integral model, so it does not perform any integration over cross-section volume.
 *
 * Note: varible cross section model uses the same keywords as simple cs model. As it is derived from simple cross section class,
 * the initializeFrom method does not calls the parent method, because the same keywords are used to read
 * variables of different type.
 */
class OOFEM_EXPORT VariableCrossSection : public SimpleCrossSection
{
protected:
    /// Expression for cross section thickness
    ScalarFunction thicknessExpr;
    /// Expression for cross section width
    ScalarFunction widthExpr;
    /// Expression for cross section area
    ScalarFunction areaExpr;
    /// Expression for cross section inertia moment $I_y$
    ScalarFunction iyExpr;
    /// Expression for cross section inertia moment $I_z$
    ScalarFunction izExpr;
    /// Expression for cross section torsion moment $I_x$
    ScalarFunction ixExpr;
    /// Expression for cross section beam shear area $A_y$
    ScalarFunction shearAreayExpr;
    /// Expression for cross section beam shear area $A_z$
    ScalarFunction shearAreazExpr;
    /// Expression for cross section beam drilling stiffness
    ScalarFunction drillingStiffnessExpr;
    /// Expression for cross section relative drilling stiffness
    ScalarFunction relDrillingStiffnessExpr;
    /// Expression for type of artificially added stiffness
    ScalarFunction drillingTypeExpr;
    /// Expression for director vector component in x-axis
    ScalarFunction directorxExpr;
    /// Expression for director vector component in y-axis
    ScalarFunction directoryExpr;
    /// Expression for director vector component in z-axis
    ScalarFunction directorzExpr;

    /// if set to true, all expressions are in element local cs, otherwise are expressed in global cs
    bool localFormulationFlag;

public:
    /**
     * Constructor.
     * @param n Cross section number.
     * @param d Associated domain.
     */
    VariableCrossSection(int n, Domain *d) : SimpleCrossSection(n, d) {
        localFormulationFlag = false;
    }

    /**
     * Initializes receiver acording to object description stored in input record.
     * Calls CrossSection initializeFrom service and reads the values of
     * - 'thick' thickness
     * - 'width' width
     * - 'area' area
     * - 'iy' Moment of inertia around y
     * - 'iz' Moment of inertia around z
     * - 'ik' Torsion moment around x
     * - 'beamshearcoeff' Beam shear coefficient
     * @param ir Record to read off.
     */
    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    const char *giveClassName() const override { return "VariableCrossSection"; }
    const char *giveInputRecordName() const override { return _IFT_VariableCrossSection_Name; }

    double give(CrossSectionProperty a, GaussPoint *gp) const override;
    double give(CrossSectionProperty a, const FloatArray &coords, Element *elem, bool local) const override;

protected:
    void giveExpression(const ScalarFunction **expr, CrossSectionProperty aProperty) const;

protected:
    std :: string giveExpression(CrossSectionProperty aProperty);
};
} // end namespace oofem
#endif // variablecrosssection_h
