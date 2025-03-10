// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "fields/eigenvectorprimaryfield.h"
#include "solvers/timestep.h"
#include "input/domain.h"
#include "dofman/dofmanager.h"
#include "dofman/dof.h"
#include "math/floatarray.h"
#include "math/floatmatrix.h"
#include "engng/engngm.h"
#include "utility/set.h"
#include "bc/boundarycondition.h"
#include "bc/initialcondition.h"
#include "input/element.h"
#include "bc/activebc.h"


namespace oofem {
EigenVectorPrimaryField :: EigenVectorPrimaryField(EngngModel *a, int idomain, FieldType ft, int nHist):
    DofDistributedPrimaryField(a, idomain, ft, nHist)
{ }


EigenVectorPrimaryField :: ~EigenVectorPrimaryField()
{ }


double
EigenVectorPrimaryField :: giveUnknownValue(Dof *dof, ValueModeType mode, TimeStep *tStep)
{
    if ( mode != VM_Total ) OOFEM_ERROR("Only VM_Total is applicable to eigen vector fields");
    return dof->giveUnknownsDictionaryValue(tStep, mode);
}


void
EigenVectorPrimaryField :: updateAll(const FloatMatrix &eigenVectors, const UnknownNumberingScheme &s)
{
    Domain *d = emodel->giveDomain(domainIndx);

    for (int i = 1; i <= eigenVectors.giveNumberOfColumns(); ++i ) {
        TimeStep step(i, nullptr, 0, 1.0, 0.0, 0);
        this->applyBoundaryCondition(&step);
    }

    auto set_values = [&eigenVectors, &s](DofManager &dman) {
        for ( Dof *dof: dman ) {
            if ( !dof->isPrimaryDof() ) continue;
            int eqNum = dof->giveEquationNumber(s);
            if ( eqNum > 0 ) {
                for (int i = 1; i <= eigenVectors.giveNumberOfColumns(); ++i ) {
                    // updateUnknownsDict should be changed to let the field have direct input rather than calling such a method.
                    // Until then, we "fake" a time step, relying on the fact that only the time-step number matters here.
                    TimeStep step(i, nullptr, 0, 1.0, 0.0, 0);
                    dof->updateUnknownsDictionary(&step, VM_Total, eigenVectors.at(eqNum, i));
                }
            }
        }
    };

    for ( auto &node : d->giveDofManagers() ) {
        set_values(*node);
    }

    for ( auto &elem : d->giveElements() ) {
        int ndman = elem->giveNumberOfInternalDofManagers();
        for ( int i = 1; i <= ndman; i++ ) {
            set_values(*elem->giveInternalDofManager(i));
        }
    }

    for ( auto &bc : d->giveBcs() ) {
        int ndman = bc->giveNumberOfInternalDofManagers();
        for ( int i = 1; i <= ndman; i++ ) {
            set_values(*bc->giveInternalDofManager(i));
        }
    }
}


void
EigenVectorPrimaryField :: applyDefaultInitialCondition()
{ }


void
EigenVectorPrimaryField :: advanceSolution(TimeStep *tStep)
{ }

} // end namespace oofem
