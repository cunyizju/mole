// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "export/solutionstatusexportmodule.h"
#include "engng/engngm.h"
#include "solvers/timestep.h"
#include "engng/classfactory.h"
#include "input/oofemtxtinputrecord.h"
#include "molecfg.h"

#include <sstream>      // std::istringstream
#include <string>       // std::string

namespace oofem {
REGISTER_ExportModule(SolutionStatusExportModule)


SolutionStatusExportModule :: SolutionStatusExportModule(int n, EngngModel *e) : ExportModule(n, e)
{
}

void
SolutionStatusExportModule :: initializeFrom(InputRecord &ir)
{
    ExportModule :: initializeFrom(ir);

    std::string formatStr("m:s:a:nite:t:dt:st:cr"); //default format
    IR_GIVE_OPTIONAL_FIELD(ir, formatStr, _IFT_SolutionStatusExportModule_format);
    // parse format string
    // use separator to read parts of the line
    std::istringstream input(formatStr);
    std::string token;
    while(std::getline(input, token, ':')) {
      recs.push_back(token);
    }
    this->checkRecs();
}

void
SolutionStatusExportModule :: doOutput(TimeStep *tStep, bool forcedOutput)
{
#if 0
    if ( !( testTimeStepOutput(tStep) || forcedOutput ) ) {
        return;
    }
#endif

    for (auto rec: this->recs) {
      if (rec == "m") {  // metstep number
        fprintf(outputFile, "%10d ", tStep->giveMetaStepNumber());
      } else if (rec == "s") { // solution step number
        fprintf(outputFile, "%10d ", tStep->giveNumber());
      } else if (rec == "a") { // attempt number
        fprintf(outputFile, "%10d ", tStep->numberOfAttempts);
      } else if (rec == "nite") { // number of iterations
        fprintf(outputFile, "%10d ", tStep->numberOfIterations); 
      } else if (rec == "t") {
        fprintf(outputFile, "%10.3e ", tStep->giveTargetTime());
      } else if (rec == "dt") {
        fprintf(outputFile, "%10.3e ", tStep->giveTimeIncrement());
      } else if (rec == "cr") {
        ConvergedReason r = tStep->convergedReason;
        std::string status;
        if (r == CR_CONVERGED) {
          status="Converged";
        } else if (r==CR_DIVERGED_ITS) {
          status="Diverged_I";
        } else if (r==CR_DIVERGED_TOL) {
          status="Diverged_T";
        } else if (r==CR_FAILED) {
          status="Failed";
        } else {
          status = "Unknown";
        }
        fprintf(outputFile, "%10s ", status.c_str());
      } else if (rec == "st") {
        fprintf(outputFile, "%10.3e ", tStep->solutionTime); 
      } else if (rec == "-") {
        fprintf(outputFile, "%10s ", "-");
      }
    }
    fprintf(outputFile, "\n");
}

void
SolutionStatusExportModule::initialize()
{
  char filename [100];    
  sprintf( filename, "%s.m%d", this->emodel->giveOutputBaseFileName().c_str(), this->number);
  this->outputFile = fopen(filename, "w");

  fprintf(outputFile,"%s (%s, %s), GitHash: %s\n", PRG_VERSION, HOST_TYPE, MODULE_LIST, MOLE_GIT_HASH);
  time_t currtime = time(NULL);
  fprintf(outputFile,"%s\n\n", ctime(& currtime));
  this->printRecsHeader();

}

void SolutionStatusExportModule::terminate()
{
  fprintf (outputFile, "\nAnalysis finished\n"); // would be nice to include completion status
  int nw, ne;
  oofem_logger.getNumberOfWarningsAndErrors(nw, ne);
  fprintf (outputFile, "Total %d error(s) and %d warnings reported\n", ne, nw);

  fclose (this->outputFile);   
}


void
SolutionStatusExportModule :: checkRecs()
{
  std::string notrecognized;
  for (auto rec: this->recs) {
    if (!(rec == "m" || rec == "s" || rec =="a" || rec=="nite" || rec == "t" ||
          rec =="dt" || rec == "cr" || rec == "st" || rec=="-")) {
      notrecognized += " " + rec;
    }
  }
  if (notrecognized.size()) {
    OOFEM_WARNING ("SolutionStatusExportModule: invalid tokens detected: %s", notrecognized.c_str());
  }
}


void
SolutionStatusExportModule :: printRecsHeader()
{
  for (auto rec: this->recs) {
    std::string label;

    if (rec == "m") {  // metstep number
      label = "MetaStep";
    } else if (rec == "s") { // solution step number
      label = "Step";
    } else if (rec == "a") { // attempt number
      label = "Attempt";
    } else if (rec == "nite") { // number of iterations
      label = "Step.nite";
    } else if (rec == "t") {
      label = "Step.t";
    } else if (rec == "dt") {
      label = "Step.dt";
    } else if (rec == "cr") {
      label = "Status";
    } else if (rec == "st") {
      label = "Solver.t";
    } else if (rec == "-") {
      label = "-";
    }
    fprintf (outputFile, "%10s ", label.c_str());
  }
  fprintf(outputFile, "\n");
}

} // end namespace oofem
