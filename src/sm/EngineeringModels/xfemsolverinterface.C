// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/EngineeringModels/xfemsolverinterface.h"
#include "sm/Elements/structuralelement.h"
#include "sm/Materials/Interface/structuralinterfacematerial.h"
#include "sm/Materials/Interface/structuralinterfacematerialstatus.h"
#include "sm/xfem/xfemstructuralelementinterface.h"
#include "sm/mappers/primvarmapper.h"
#include "solvers/timestep.h"
#include "structengngmodel.h"
#include "staticstructural.h"
#include "fields/primaryfield.h"
#include "input/domain.h"
#include "xfem/xfemmanager.h"
#include "input/element.h"
#include "mapping/matstatmapperint.h"
#include "input/nummet.h"
#include "math/floatarray.h"
#include "export/exportmodulemanager.h"
#include "export/vtkxmlexportmodule.h"

namespace oofem {

XfemSolverInterface::XfemSolverInterface() :
    mNeedsVariableMapping(false)
{ }

void XfemSolverInterface::propagateXfemInterfaces(TimeStep *tStep, StructuralEngngModel &ioEngngModel, bool iRecomputeStepAfterCrackProp)
{
    int domainInd = 1;
    Domain *domain = ioEngngModel.giveDomain(domainInd);

    if ( domain->hasXfemManager() ) {
        XfemManager *xMan = domain->giveXfemManager();
        bool frontsHavePropagated = false;
        if ( xMan->hasInitiationCriteria() ) {
            // TODO: generalise this?
            // Intitiate delaminations (only implemented for listbasedEI/delamination. Treated the same way as propagation)
            xMan->initiateFronts(frontsHavePropagated,tStep);
        }

        if ( xMan->hasPropagatingFronts() ) {
            // Propagate crack tips
            xMan->propagateFronts(frontsHavePropagated);
        }

        bool eiWereNucleated = false;
        if ( xMan->hasNucleationCriteria() ) {
        	xMan->nucleateEnrichmentItems(eiWereNucleated);
        }

        for ( auto &elem : domain->giveElements() ) {
            ////////////////////////////////////////////////////////
            // Map state variables for enriched elements
            XfemElementInterface *xfemElInt = dynamic_cast< XfemElementInterface * >( elem.get() );

            if ( xfemElInt ) {
                xfemElInt->XfemElementInterface_updateIntegrationRule();
            }
        }

        if ( frontsHavePropagated || eiWereNucleated ) {
            mNeedsVariableMapping = false;

            ioEngngModel.giveDomain(1)->postInitialize();
            ioEngngModel.forceEquationNumbering();

            if ( iRecomputeStepAfterCrackProp ) {
                OOFEM_LOG_RELEVANT("Recomputing time step.\n");
                ioEngngModel.forceEquationNumbering();
                ioEngngModel.solveYourselfAt(tStep);
                ioEngngModel.updateYourself( tStep );
                ioEngngModel.terminate( tStep );
            }
        }
    }
}


} /* namespace oofem */
