// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "adaptlinearstatic.h"
#include "mesher/remeshingcrit.h"
#include "mesher/mesherinterface.h"
#include "error/errorestimator.h"
#include "input/domain.h"
#include "engng/classfactory.h"
#include "utility/contextioerr.h"
#include "export/outputmanager.h"

namespace oofem {
REGISTER_EngngModel(AdaptiveLinearStatic);

void
AdaptiveLinearStatic :: updateYourself(TimeStep *tStep)
{
    LinearStatic :: updateYourself(tStep);
    // perform error evaluation
    // evaluate error of the reached solution
    this->defaultErrEstimator->estimateError(temporaryEM, tStep);
    // this->defaultErrEstimator->estimateError (equilibratedEM, this->giveCurrentStep());
    RemeshingStrategy strategy = this->defaultErrEstimator->giveRemeshingCrit()->giveRemeshingStrategy(tStep);

    if ( strategy == NoRemeshing_RS ) {
        return;
    } else {
        // do remeshing
        std :: unique_ptr< MesherInterface >mesher( classFactory.createMesherInterface( meshPackage, this->giveDomain(1) ) );
        Domain *newDomain;

        MesherInterface :: returnCode result =
            mesher->createMesh(tStep, 1, this->giveDomain(1)->giveSerialNumber() + 1, & newDomain);

        if ( result == MesherInterface :: MI_OK ) { } else if ( result == MesherInterface :: MI_NEEDS_EXTERNAL_ACTION ) {
            // terminate step
            //this->terminate( tStep );
            //this->terminateAnalysis();
            //exit(1);
        } else {
            OOFEM_ERROR("createMesh failed");
        }
    }
}


void
AdaptiveLinearStatic :: printOutputAt(FILE *file, TimeStep *tStep)
{
    if ( !this->giveDomain(1)->giveOutputManager()->testTimeStepOutput(tStep) ) {
        return;
    }

    LinearStatic :: printOutputAt(file, tStep);
    fprintf(file, "\nRelative error estimate: %5.2f%%\n", this->defaultErrEstimator->giveValue(relativeErrorEstimateEEV, tStep) * 100.0);
}


int
AdaptiveLinearStatic :: initializeAdaptive(int tStepNumber)
{
    /*
     * Due to linear character of the problem,
     * the whole analysis is restarted from beginning.
     * The solution steps represent the adaptive steps and for each adaptive step
     * new domain with corresponding domainSerNum is generated.
     */
    int result = 1;
    /*
     * this -> initStepIncrements();
     *
     * int sernum = tStepNumber + 1;
     * printf ("\nrestoring domain %d.%d\n", 1, sernum);
     * Domain* dNew = new Domain (1, sernum, this);
     * FILE* domainInputFile;
     * this->giveDomainFile (&domainInputFile, 1, sernum, contextMode_read);
     * if (!dNew -> instanciateYourself(domainInputFile)) OOFEM_ERROR("domain Instanciation failed");
     * fclose (domainInputFile);
     *
     * printf ("\ndeleting old domain\n");
     * delete domainList->at(1);
     * domainList->put(1, dNew);
     *
     * // init equation numbering
     * this->forceEquationNumbering();
     *
     * // set time step
     * this->giveCurrentStep()->setTime(tStepNumber+1);
     *
     * // init equation numbering
     * // this->forceEquationNumbering();
     * this->giveNumericalMethod(giveCurrentStep())->setDomain (dNew);
     * this->ee->setDomain (dNew);
     */
    return result;
}


void AdaptiveLinearStatic :: restoreContext(DataStream &stream, ContextMode mode)
{
    LinearStatic :: restoreContext(stream, mode);
}

void
AdaptiveLinearStatic :: initializeFrom(InputRecord &ir)
{
    LinearStatic :: initializeFrom(ir);

    int meshPackageId = 0;
    IR_GIVE_OPTIONAL_FIELD(ir, meshPackageId, _IFT_AdaptiveLinearStatic_meshpackage);

    if ( meshPackageId == 1 ) {
        meshPackage = MPT_TARGE2;
    } else if ( meshPackageId == 2 ) {
        meshPackage = MPT_FREEM;
    } else {
        meshPackage = MPT_T3D;
    }
}


void
AdaptiveLinearStatic :: updateDomainLinks()
{
    LinearStatic :: updateDomainLinks();
    // associate ee to possibly newly restored mesh
    this->defaultErrEstimator->setDomain( this->giveDomain(1) );
}
} // end namespace oofem
