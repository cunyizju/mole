// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef elementside_h
#define elementside_h

#include "dofman/dofmanager.h"

#define _IFT_ElementSide_Name "elementside"

namespace oofem {
class Dof;
class NodalLoad;
class TimeStep;
class FloatArray;
class IntArray;

/**
 * Class implementing element side having some DOFs in finite element mesh.
 * ElementSide possess degrees of freedom (see base class DofManager).
 * ElementSide id usually attribute of few elements and is managed by domain.
 *
 * Tasks:
 * - managing its degrees of freedom (giveDof).
 * - calculating its  load vector.
 * - printing and updating at end of step.
 * - managing its swapping to and from disk.
 */
class OOFEM_EXPORT ElementSide : public DofManager
{
public:
    /**
     * Constructor. Creates a element side belonging to domain.
     * @param n Side number in domain aDomain
     * @param aDomain Domain to which side belongs
     */
    ElementSide(int n, Domain * aDomain);
    /// Destructor.
    virtual ~ElementSide();

    const char *giveClassName() const override { return "ElementSide"; }
    const char *giveInputRecordName() const override { return _IFT_ElementSide_Name; }
    void initializeFrom(InputRecord &ir) override;
    void printYourself() override;

    /**
     * Computes receiver transformation matrix from global cs. to dofManager specific
     * coordinate system (in which governing equations are assembled, for example the
     * local coordinate system in node).
     * @param answer Computed transformation matrix. It has generally dofIDArry.size rows and
     * if loc is obtained using giveLocationArray(dofIDArry, loc) call, loc.giveSize() columns.
     * This is because this transformation should generally include not only transformation to
     * dof manager local coordinate system, but receiver dofs can be expressed using
     * dofs of another dofManager (In this case, square answer is produced only if all
     * dof transformation is required).
     * @param dofIDArry Array containing DofIDItem values for which transformation matrix is
     * assembled. If dofIDArry is NULL, then all receiver dofs are assumed.
     */
    virtual void computeTransformation(FloatMatrix &answer, const IntArray *dofIDArry);
    bool requiresTransformation() override { return false; }
    bool isDofTypeCompatible(dofType type) const override { return ( type == DT_master || type == DT_simpleSlave ); }
};
} // end namespace oofem
#endif // elementside_h
