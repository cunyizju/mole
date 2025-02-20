// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include <vector>
#include <cstdio>

#include "export/outputexportmodule.h"
#include "engng/engngm.h"
#include "input/domain.h"
#include "dofman/node.h"
#include "input/element.h"
#include "solvers/timestep.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_ExportModule(OutputExportModule)

OutputExportModule :: OutputExportModule(int n, EngngModel *e) : ExportModule(n, e), outputStream(NULL)
{
}

void
OutputExportModule :: initializeFrom(InputRecord &ir)
{
    nodeSets.clear();
    IR_GIVE_OPTIONAL_FIELD(ir, nodeSets, _IFT_OutputExportModule_nodeSets);
    elementSets.clear();
    IR_GIVE_OPTIONAL_FIELD(ir, elementSets, _IFT_OutputExportModule_elementSets);

    ExportModule :: initializeFrom(ir);

    auto *file = giveOutputStream();

    fprintf(file, "%s", PRG_HEADER);
    fprintf(file, "\nStarting analysis on: %s\n", ctime(& emodel->giveStartTime()) );
    fprintf(file, "%s\n", emodel->giveDescription().c_str());
}

FILE *
OutputExportModule :: giveOutputStream()
{
    if ( this->outputStream ) return this->outputStream;
    this->outputStream = fopen(emodel->giveOutputBaseFileName().c_str(), "w");
    if ( !this->outputStream ) {
        OOFEM_ERROR("Can't open output file %s", emodel->giveOutputBaseFileName().c_str());
    }
    return this->outputStream;
}

void
OutputExportModule :: doOutput(TimeStep *tStep, bool forcedOutput)
{
    if ( !( testTimeStepOutput(tStep) || forcedOutput ) ) {
        return;
    }

    FILE *file = this->giveOutputStream();

    fprintf(file, "\n==============================================================");
    fprintf(file, "\nOutput for time %.8e ", tStep->giveTargetTime() * emodel->giveVariableScale(VST_Time) );
    fprintf(file, "\n==============================================================\n");

    emodel->printOutputAt(file, tStep, nodeSets, elementSets);

    fprintf(file, "\nUser time consumed by solution step %d: %.3f [s]\n\n", tStep->giveNumber(), emodel->giveSolutionStepTime());
}

void
OutputExportModule :: terminate()
{
    FILE *out = this->giveOutputStream();
    int rhrs, rmin, rsec, uhrs, umin, usec;
    time_t endTime = time(NULL);

    emodel->giveAnalysisTime(rhrs, rmin, rsec, uhrs, umin, usec);

    fprintf(out, "\nFinishing analysis on: %s\n", ctime(& endTime) );
    fprintf(out, "Real time consumed: %03dh:%02dm:%02ds\n", rhrs, rmin, rsec);
    fprintf(out, "User time consumed: %03dh:%02dm:%02ds\n\n\n", uhrs, umin, usec);
}

} // end namespace oofem
