// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "export/gpexportmodule.h"
#include "math/gausspoint.h"
#include "material/material.h"
#include "input/element.h"
#include "math/integrationrule.h"
#include "solvers/timestep.h"
#include "engng/engngm.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_ExportModule(GPExportModule)

GPExportModule :: GPExportModule(int n, EngngModel *e) : ExportModule(n, e)
{
    ncoords = -1; // means: export as many coordinates as available
}


GPExportModule :: ~GPExportModule()
{ }


void
GPExportModule :: initializeFrom(InputRecord &ir)
{
    ExportModule :: initializeFrom(ir);
    IR_GIVE_FIELD(ir, vartypes, _IFT_GPExportModule_vartypes);
    IR_GIVE_OPTIONAL_FIELD(ir, ncoords, _IFT_GPExportModule_ncoords);
}


void
GPExportModule :: doOutput(TimeStep *tStep, bool forcedOutput)
{
    if ( !testTimeStepOutput(tStep) ) {
        return;
    }

    double weight;
    FloatArray gcoords, intvar;

    Domain *d = emodel->giveDomain(1);
    FILE *stream = this->giveOutputStream(tStep);

    // print the header
    fprintf(stream, "%%# gauss point data file\n");
    fprintf(stream, "%%# output for time %g\n", tStep->giveTargetTime() );
    fprintf(stream, "%%# variables: ");
    fprintf(stream, "%d  ", vartypes.giveSize());
    for ( auto &vartype : vartypes ) {
        fprintf( stream, "%d ", vartype );
    }

    fprintf(stream, "\n %%# for interpretation see internalstatetype.h\n");
    
    elements.resize(0);
    for ( int ireg = 1; ireg <= this->giveNumberOfRegions(); ireg++ ) {
        elements.followedBy(this->giveRegionSet(ireg)->giveElementList());
    }
    
    // loop over elements
    for ( auto &elem : d->giveElements() ) {
        if ( elements.contains(elem -> giveNumber()) ){
            
            //iRule = elem->giveDefaultIntegrationRulePtr();
            //int numIntRules = elem->giveNumberOfIntegrationRules();
            for ( int i = 0; i < elem->giveNumberOfIntegrationRules(); i++ ) {
                IntegrationRule *iRule = elem->giveIntegrationRule(i);

                // loop over Gauss points
                for ( GaussPoint *gp: *iRule ) {
                    // export:
                    // 1) element number
                    // 2) material number ///@todo deprecated returns -1
                    // 3) Integration rule number
                    // 4) Gauss point number
                    // 5) contributing volume around Gauss point
                    weight = elem->computeVolumeAround(gp);
                    fprintf(stream, "%d %d %d %d %.6e ", elem->giveNumber(), -1, i + 1, gp->giveNumber(), weight);

                    // export Gauss point coordinates
                    if ( ncoords ) { // no coordinates exported if ncoords==0
                        elem->computeGlobalCoordinates( gcoords, gp->giveNaturalCoordinates() );
                        int nc = gcoords.giveSize();
                        if ( ncoords >= 0 ) {
                            fprintf(stream, "%d ", ncoords);
                        } else {
                            fprintf(stream, "%d ", nc);
                        }

                        if ( ncoords > 0 && ncoords < nc ) {
                            nc = ncoords;
                        }

                        for ( auto &c : gcoords ) {
                            fprintf( stream, "%.6e ", c );
                        }

                        for ( int ic = nc + 1; ic <= ncoords; ic++ ) {
                            fprintf(stream, "%g ", 0.0);
                        }
                    }

                    // export internal variables
                    for ( auto vartype : vartypes ) {
                        elem->giveIPValue(intvar, gp, ( InternalStateType )vartype, tStep);
                        fprintf(stream, "%d ", intvar.giveSize());
                        for ( auto &val : intvar ) {
                            fprintf( stream, "%.6e ", val );
                        }
                    }

                    fprintf(stream, "\n");
                }
            }

    #if 0
            // for CST elements write also nodal coordinates
            // (non-standard part, used only exceptionally)
            int nnode = elem->giveNumberOfNodes();
            if ( nnode == 3 ) {
                for ( int inod = 1; inod <= 3; inod++ ) {
                    fprintf( stream, "%f %f ", elem->giveNode(inod)->giveCoordinate(1), elem->giveNode(inod)->giveCoordinate(2) );
                }
            }
#endif
        }
    }
    fclose(stream);
}

void
GPExportModule :: initialize()
{ 
    ExportModule :: initialize();
}


void
GPExportModule :: terminate()
{ }


FILE *
GPExportModule :: giveOutputStream(TimeStep *tStep)
{
    FILE *answer;

    std :: string fileName = this->giveOutputBaseFileName(tStep) + ".gp";
    if ( ( answer = fopen(fileName.c_str(), "w") ) == NULL ) {
        OOFEM_ERROR("failed to open file %s", fileName.c_str());
    }

    return answer;
}
} // namespace oofem
