// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li


#ifndef prescribedgradientperiodic_h_
#define prescribedgradientperiodic_h_

#include "bc/prescribedgradienthomogenization.h"
#include "bc/activebc.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"

#include <memory>
#include <map>

#define _IFT_PrescribedGradientBCPeriodic_Name "prescribedgradientperiodic"
#define _IFT_PrescribedGradientBCPeriodic_masterSet "masterset"
#define _IFT_PrescribedGradientBCPeriodic_jump "jump"

namespace oofem {
class Node;

/**
 * Prescribes an average displacement gradient based on microperiodicity.
 * @note Nodes on either master or slave side may NOT have local coordinate systems.
 *
 * @author Mikael Öhman
 */
class OOFEM_EXPORT PrescribedGradientBCPeriodic : public ActiveBoundaryCondition, public PrescribedGradientHomogenization
{
protected:
    std :: unique_ptr< Node > strain;
    IntArray strain_id;

    std :: map< int, int > slavemap;
    FloatArray jump;

    int masterSet;

    /**
     * This is the central support function, which finds the corresponding slave nodes for each master node.
     */
    void findSlaveToMasterMap();

public:
    PrescribedGradientBCPeriodic(int n, Domain *d);
    virtual ~PrescribedGradientBCPeriodic();

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;
    void postInitialize() override;

    int giveNumberOfInternalDofManagers() override;
    DofManager *giveInternalDofManager(int i) override;

    int giveNumberOfMasterDofs(ActiveDof *dof) override;
    Dof *giveMasterDof(ActiveDof *dof, int mdof) override;

    void computeDofTransformation(ActiveDof *dof, FloatArray &masterContribs) override;
    void computeField(FloatArray &sigma, TimeStep *tStep) override;
    void computeTangent(FloatMatrix &E, TimeStep *tStep) override;
    double giveUnknown(PrimaryField &field, ValueModeType mode, TimeStep *tStep, ActiveDof *dof) override;
    double giveUnknown(ValueModeType mode, TimeStep *tStep, ActiveDof *dof) override;
    double giveUnknown(double val, ValueModeType mode, TimeStep *tStep, ActiveDof *dof);
    bool isPrimaryDof(ActiveDof *dof) override;
    double giveBcValue(Dof *dof, ValueModeType mode, TimeStep *tStep) override;
    bool hasBc(Dof *dof, TimeStep *tStep) override;
    bool isStrainDof(Dof *dof);

    bool requiresActiveDofs() override { return true; }

    const char *giveClassName() const override { return "PrescribedGradientBCPeriodic"; }
    const char *giveInputRecordName() const override { return _IFT_PrescribedGradientBCPeriodic_Name; }
};
} // end namespace oofem

#endif // prescribedgradientperiodic_h_
