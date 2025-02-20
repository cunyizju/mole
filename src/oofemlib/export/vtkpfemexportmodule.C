// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "export/vtkpfemexportmodule.h"
#include "input/element.h"
#include "math/gausspoint.h"
#include "solvers/timestep.h"
#include "engng/engngm.h"
#include "dofman/node.h"
#include "dofman/dof.h"
#include "material/materialinterface.h"
#include "math/mathfem.h"
#include "input/cltypes.h"
#include "material/material.h"
#include "engng/classfactory.h"
#include "cs/crosssection.h"
#include "input/unknownnumberingscheme.h"

#ifdef __PFEM_MODULE
 #include "pfem/pfemparticle.h"
#endif

#include <string>
#include <sstream>
#include <ctime>

namespace oofem {
REGISTER_ExportModule(VTKPFEMXMLExportModule)


// Begin VTKPFEMXMLExportModule

VTKPFEMXMLExportModule::VTKPFEMXMLExportModule(int n, EngngModel *e) : VTKBaseExportModule(n, e) {}

VTKPFEMXMLExportModule::~VTKPFEMXMLExportModule() { }

void
VTKPFEMXMLExportModule::initializeFrom(InputRecord &ir)
{
    ExportModule::initializeFrom(ir);
}

std::string
VTKPFEMXMLExportModule::giveOutputFileName(TimeStep *tStep)
{
    return this->giveOutputBaseFileName(tStep) + ".vtu";
}


std::ofstream
VTKPFEMXMLExportModule::giveOutputStream(TimeStep *tStep)
{
    std::string fileName = giveOutputFileName(tStep);
    std::ofstream streamF;

    if ( pythonExport ) {
        streamF = std::ofstream(NULL_DEVICE);//do not write anything
    } else {
        streamF = std::ofstream(fileName);
    }

    if ( !streamF.good() ) {
        OOFEM_ERROR("failed to open file %s", fileName.c_str() );
    }

    streamF.fill('0');//zero padding
    return streamF;
}


void
VTKPFEMXMLExportModule::doOutput(TimeStep *tStep, bool forcedOutput)
{
    if ( !( testTimeStepOutput(tStep) || forcedOutput ) ) {
        return;
    }
    
    this->fileStream = this->giveOutputStream(tStep);
    struct tm *current;
    time_t now;
    time(& now);
    current = localtime(& now);

    // Write output: VTK header
    this->fileStream << "<!-- TimeStep " << tStep->giveTargetTime() * timeScale << " Computed " << current->tm_year + 1900 << "-" << setw(2) << current->tm_mon + 1 << "-" << setw(2) << current->tm_mday << " at " << current->tm_hour << ":" << current->tm_min << ":" << setw(2) << current->tm_sec << " -->\n";
    this->fileStream << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n";
    this->fileStream << "<UnstructuredGrid>\n";

#ifdef __PFEM_MODULE
    // write out the particles (nodes exported as vertices = VTK_VERTEX)
    Domain *d  = emodel->giveDomain(1);
    int nnode = d->giveNumberOfDofManagers();

    int nActiveNode = 0;
    for ( int inode = 1; inode <= nnode; inode++ ) {
        PFEMParticle *particle = dynamic_cast< PFEMParticle * >( d->giveNode(inode) );
        if ( particle ) {
            if ( particle->isActive() ) {
                nActiveNode++;
            }
        }
    }

    DofManager *node;
    FloatArray *coords;
    this->fileStream << "<Piece NumberOfPoints=\"" << nActiveNode << "\" NumberOfCells=\"" << nActiveNode << "\">\n";
    this->fileStream << "<Points>\n <DataArray type=\"Float64\" NumberOfComponents=\"3\" format=\"ascii\"> ";

    for ( int inode = 1; inode <= nnode; inode++ ) {
        node = d->giveNode(inode);
        PFEMParticle *particle = dynamic_cast< PFEMParticle * >( node );
        if ( particle ) {
            if ( particle->isActive() ) {
                coords = node->giveCoordinates();
                ///@todo move this below into setNodeCoords since it should alwas be 3 components anyway
                for ( int i = 1; i <= coords->giveSize(); i++ ) {
                    this->fileStream << scientific << coords->at(i) << " ";
                }

                for ( int i = coords->giveSize() + 1; i <= 3; i++ ) {
                    this->fileStream << scientific << 0.0 << " ";
                }
            }
        }
    }

    this->fileStream << "</DataArray>\n</Points>\n";


    // output the cells connectivity data
    this->fileStream << "<Cells>\n";
    this->fileStream << " <DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\"> ";

    for ( int ielem = 1; ielem <= nActiveNode; ielem++ ) {
        this->fileStream << ielem - 1 << " ";
    }

    this->fileStream << "</DataArray>\n";

    // output the offsets (index of individual element data in connectivity array)
    this->fileStream << " <DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\"> ";

    for ( int ielem = 1; ielem <= nActiveNode; ielem++ ) {
        this->fileStream << ielem << " ";
    }
    this->fileStream << "</DataArray>\n";


    // output cell (element) types
    this->fileStream << " <DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\"> ";
    for ( int ielem = 1; ielem <= nActiveNode; ielem++ ) {
        this->fileStream << 1 << " ";
    }

    this->fileStream << "</DataArray>\n";
    this->fileStream << "</Cells>\n";
    this->fileStream << "</Piece>\n";
#endif //__PFEM_MODULE

    // Finalize the output:
    this->fileStream << "</UnstructuredGrid>\n</VTKFile>";
    if(this->fileStream){
        this->fileStream.close();
    }

 }

} // end namespace oofem
