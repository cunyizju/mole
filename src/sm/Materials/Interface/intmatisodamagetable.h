// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef intmatisodamagetable_h
#define intmatisodamagetable_h


#include "intmatisodamage.h"
#include <fstream>

///@name Input fields for IntMatIsoDamageTable
//@{
#define _IFT_IntMatIsoDamageTable_Name "intmatisodamagetable"
#define _IFT_IntMatIsoDamageTable_kn "kn"
#define _IFT_IntMatIsoDamageTable_ks "ks"
#define _IFT_IntMatIsoDamageTable_ft "ft"
#define _IFT_IntMatIsoDamageTable_tablename "tablename"
#define _IFT_IntMatIsoDamageTable_maxOmega "maxomega"

//@}

namespace oofem {

/**
 * Simple isotropic damage based model for 2d and 3d interface elements.
 * In 2d, the interface elements are used to model contact layer between
 * element edges. The generalized strain vector contains two relative displacements
 * (in normal and shear direction). The generalized stress vector contains corresponding
 * tractions in normal and tangent direction.
 *
 * In 3d, the interface elements are used to model contact layer between
 * element surfaces. The generalized strain vector contains two relative displacements
 * (in normal and shear direction). The generalized stress vector contains corresponding
 * tractions in normal and tangent direction
 *
 * The behaviour of the model is elastic, described by normal and shear stiffness components.
 * Isotropic damage is initiated  when the stress reaches the tensile strength. Damage evolution
 * is governed by normal component of generalized strain vector (normal relative displacement)
 * by a table given by a file that relates the normal displacement to the damage. A linear interpolation
 * is made between the values given in the table. If the strain is greater than the largest value
 * in the table the largest damage in the table will be used.
 *
 * Differences between this class and IsoInterfaceDamageMaterial written by:
 * @author Kristoffer Carlsson
 * @author Jim Brouzoulis
 */

class IntMatIsoDamageTable : public IntMatIsoDamage
{
protected:
    /// Name of table file
    std :: string tablename;
    /// Damages read from the second column in the table file
    FloatArray tableDamages;
    /// Jumps read from the first column in the table file
    FloatArray tableJumps;

public:
    /// Constructor
    IntMatIsoDamageTable(int n, Domain *d);

    const char *giveClassName() const override { return "IntMatIsoDamageTable"; }
    const char *giveInputRecordName() const override { return _IFT_IntMatIsoDamageTable_Name; }

    double computeDamageParam(double kappa) const override;

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;
};
} // end namespace oofem
#endif // isointerfacedamage01_h
