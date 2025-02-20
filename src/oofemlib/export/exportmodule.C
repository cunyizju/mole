// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "export/exportmodule.h"
#include "solvers/timestep.h"
#include "engng/engngm.h"
#include "input/domain.h"

#include <cstdarg>


namespace oofem {
ExportModule :: ExportModule(int n, EngngModel *e) : tsteps_out(), domainMask(), regionSets(), defaultElementSet( 0, e->giveDomain(1) )
{
    this->number = n;
    emodel = e;
    regionSets.resize(0);
    timeScale = 1.;
    pythonExport = false;
}


ExportModule :: ~ExportModule()
{ }


void
ExportModule :: initializeFrom(InputRecord &ir)
{
    tstep_all_out_flag = ir.hasField(_IFT_ExportModule_tstepall);

    tstep_step_out = 0;
    IR_GIVE_OPTIONAL_FIELD(ir, tstep_step_out, _IFT_ExportModule_tstepstep);

    IR_GIVE_OPTIONAL_FIELD(ir, tsteps_out, _IFT_ExportModule_tstepsout);

    tstep_substeps_out_flag = 0;
    IR_GIVE_OPTIONAL_FIELD(ir, tstep_substeps_out_flag, _IFT_ExportModule_subtstepsout);

    IR_GIVE_OPTIONAL_FIELD(ir, pythonExport, _IFT_ExportModule_pythonexport);
    
    domain_all_flag = ir.hasField(_IFT_ExportModule_domainall);

    if ( !domain_all_flag ) {
        domainMask.clear();
        IR_GIVE_OPTIONAL_FIELD(ir, domainMask, _IFT_ExportModule_domainmask);
    }

    IR_GIVE_OPTIONAL_FIELD(ir, regionSets, _IFT_ExportModule_regionsets); // Macro

    IR_GIVE_OPTIONAL_FIELD(ir, timeScale, _IFT_ExportModule_timescale); // Macro
}

void ExportModule :: initialize(){
  initializeElementSet();
}

void ExportModule :: initializeElementSet(){
    if ( regionSets.isEmpty() ){
        // default: whole domain region
        defaultElementSet.clear();
        defaultElementSet.setDomain(emodel->giveDomain(1));
        defaultElementSet.addAllElements();
    }
}


int ExportModule :: giveNumberOfRegions()
{
    // Returns number of regions (aka sets)
  if (regionSets.isEmpty())
    return 1; // defaultElementSet
  else 
    return this->regionSets.giveSize();
}

Set *ExportModule :: giveRegionSet(int i)
{
  if (regionSets.isEmpty()) {
    return & this->defaultElementSet;
  }  else {
    int setid = regionSets.at(i);
    if ( setid > 0 ) {
        return emodel->giveDomain(1)->giveSet(setid);
    } else {
        OOFEM_ERROR ("Bad set index");
        return & this->defaultElementSet;
    }
  }
}

std :: string
ExportModule :: giveOutputBaseFileName(TimeStep *tStep)
{
    char fext [ 100 ];

    if ( this->testSubStepOutput() ) {
        // include tStep version in output file name
        if ( this->emodel->isParallel() && this->emodel->giveNumberOfProcesses() > 1 ) {
            sprintf( fext, "_%03d.m%d.%d.%d", emodel->giveRank(), this->number, tStep->giveNumber(), tStep->giveSubStepNumber() );
        } else {
            sprintf( fext, ".m%d.%d.%d", this->number, tStep->giveNumber(), tStep->giveSubStepNumber() );
        }
        return this->emodel->giveOutputBaseFileName() + fext;
    } else {
        if ( this->emodel->isParallel() && this->emodel->giveNumberOfProcesses() > 1 ) {
            sprintf( fext, "_%03d.m%d.%d", emodel->giveRank(), this->number, tStep->giveNumber() );
        } else {
            sprintf( fext, ".m%d.%d", this->number, tStep->giveNumber() );
        }
        return this->emodel->giveOutputBaseFileName() + fext;
    }
}

bool
ExportModule :: testTimeStepOutput(TimeStep *tStep)
{
    if ( tstep_all_out_flag ) {
        return true;
    }

    if ( tstep_step_out ) {
        //if (((tStep->giveNumber()-emodel->giveNumberOfFirstStep()) % tstep_step_out) == 0) return 1;
        if ( ( ( tStep->giveNumber() ) % tstep_step_out ) == 0 ) {
            return 1;
        }
    }

    for ( auto &step: tsteps_out ) {
        // test if INCLUDED
        if ( step.test( tStep->giveNumber() ) ) {
            return true;
        }
    }

    return 0;
}

bool
ExportModule :: testDomainOutput(int n)
{
    if ( domain_all_flag ) {
        return true;
    }

    return domainMask.findFirstIndexOf(n);
}

std :: string ExportModule :: errorInfo(const char *func) const
{
    return std :: string(this->giveClassName()) + "::" + func;
}
} // end namespace oofem
