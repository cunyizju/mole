// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/PlaneStress/planstrssxfem.h"
#include "sm/Materials/Structural/structuralmaterial.h"
#include "sm/CrossSections/structuralcrosssection.h"
#include "xfem/xfemelementinterface.h"
#include "xfem/enrichmentfunction.h"
#include "xfem/enrichmentitem.h"
#include "export/vtkxmlexportmodule.h"
#include "input/dynamicinputrecord.h"
#include "fei/feinterpol.h"
#include "engng/classfactory.h"
#include "math/mathfem.h"



namespace oofem {
REGISTER_Element(PlaneStress2dXfem)

void PlaneStress2dXfem :: updateYourself(TimeStep *tStep)
{
    PlaneStress2d :: updateYourself(tStep);
    XfemStructuralElementInterface :: updateYourselfCZ(tStep);
}

void PlaneStress2dXfem :: postInitialize()
{
    PlaneStress2d :: postInitialize();
    XfemStructuralElementInterface :: initializeCZMaterial();
}

Interface *
PlaneStress2dXfem :: giveInterface(InterfaceType it)
{
    if ( it == XfemElementInterfaceType ) {
        return static_cast< XfemElementInterface * >(this);
    } else if ( it == VTKXMLExportModuleElementInterfaceType ) {
        return static_cast< VTKXMLExportModuleElementInterface * >(this);
    } else {
        return PlaneStress2d :: giveInterface(it);
    }
}


void
PlaneStress2dXfem :: computeGaussPoints()
{
    if ( this->giveDomain()->hasXfemManager() ) {
        XfemManager *xMan = this->giveDomain()->giveXfemManager();

        if ( xMan->isElementEnriched(this) ) {
            if ( !this->XfemElementInterface_updateIntegrationRule() ) {

                // Blending element
//                increaseNumGP();
                PlaneStress2d :: computeGaussPoints();
            }
        } else {
            PlaneStress2d :: computeGaussPoints();
        }
    } else   {
        PlaneStress2d :: computeGaussPoints();
    }
}

void PlaneStress2dXfem :: increaseNumGP()
{
    switch(numberOfGaussPoints)
    {
    case(4):
        numberOfGaussPoints = 9;
        break;
    case(9):
        numberOfGaussPoints = 16;
        break;
    case(16):
        numberOfGaussPoints = 25;
        break;
    }
}

void PlaneStress2dXfem :: computeBmatrixAt(GaussPoint *gp, FloatMatrix &answer, int li, int ui)
{
    XfemElementInterface_createEnrBmatrixAt(answer, * gp, * this);
}

void PlaneStress2dXfem :: computeBHmatrixAt(GaussPoint *gp, FloatMatrix &answer)
{
    XfemElementInterface_createEnrBHmatrixAt(answer, * gp, * this);
}


void PlaneStress2dXfem :: computeNmatrixAt(const FloatArray &iLocCoord, FloatMatrix &answer)
{
    XfemElementInterface_createEnrNmatrixAt(answer, iLocCoord, * this, false);
}



int PlaneStress2dXfem :: computeNumberOfDofs()
{
    int ndofs = 0;
    for ( int i = 1; i <= this->giveNumberOfDofManagers(); i++ ) {
        ndofs += this->giveDofManager(i)->giveNumberOfDofs();
    }

    return ndofs;
}


void
PlaneStress2dXfem :: giveDofManDofIDMask(int inode, IntArray &answer) const
{
    // Continuous part
    PlaneStress2d :: giveDofManDofIDMask(inode, answer);

    // Discontinuous part
    if( this->giveDomain()->hasXfemManager() ) {
        DofManager *dMan = giveDofManager(inode);
        XfemManager *xMan = giveDomain()->giveXfemManager();

        const std::vector<int> &nodeEiIndices = xMan->giveNodeEnrichmentItemIndices( dMan->giveGlobalNumber() );
        for ( size_t i = 0; i < nodeEiIndices.size(); i++ ) {
            EnrichmentItem *ei = xMan->giveEnrichmentItem(nodeEiIndices[i]);
            if ( ei->isDofManEnriched(* dMan) ) {
                IntArray eiDofIdArray;
                ei->computeEnrichedDofManDofIdArray(eiDofIdArray, *dMan);
                answer.followedBy(eiDofIdArray);
            }
        }
    }
}



void PlaneStress2dXfem :: computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep)
{
    XfemStructuralElementInterface :: XfemElementInterface_computeConstitutiveMatrixAt(answer, rMode, gp, tStep);
}

void
PlaneStress2dXfem :: computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep)
{
    XfemStructuralElementInterface :: XfemElementInterface_computeStressVector(answer, strain, gp, tStep);
}

void PlaneStress2dXfem :: computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep)
{
    PlaneStress2d :: computeStiffnessMatrix(answer, rMode, tStep);
    XfemStructuralElementInterface :: computeCohesiveTangent(answer, tStep);

    const double tol = 1.0e-6;
    const double regularizationCoeff = 1.0e-6;
    int numRows = answer.giveNumberOfRows();
    for(int i = 0; i < numRows; i++) {
        if( fabs(answer(i,i)) < tol ) {
            answer(i,i) += regularizationCoeff;
            //printf("Found zero on diagonal.\n");
        }
    }
}

void PlaneStress2dXfem :: computeDeformationGradientVector(FloatArray &answer, GaussPoint *gp, TimeStep *tStep)
{
    XfemStructuralElementInterface::XfemElementInterface_computeDeformationGradientVector(answer, gp, tStep);
}

void
PlaneStress2dXfem :: giveInternalForcesVector(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord)
{
    PlaneStress2d :: giveInternalForcesVector(answer, tStep, useUpdatedGpRecord);
    XfemStructuralElementInterface :: computeCohesiveForces(answer, tStep);
}

Element_Geometry_Type
PlaneStress2dXfem :: giveGeometryType() const
{
    if ( this->giveDomain()->hasXfemManager() ) {
        XfemManager *xMan = this->giveDomain()->giveXfemManager();
        if ( xMan->isElementEnriched(this) ) {
            return EGT_Composite;
            //return EGT_quad_1;
        } else {
            return EGT_Composite;
            //return EGT_quad_1;
        }
    } else   {
        return EGT_quad_1;
    }
}

void
PlaneStress2dXfem :: initializeFrom(InputRecord &ir)
{
    PlaneStress2d :: initializeFrom(ir);
    XfemStructuralElementInterface :: initializeCZFrom(ir);
}

MaterialMode PlaneStress2dXfem :: giveMaterialMode()
{
    return XfemStructuralElementInterface :: giveMaterialMode();
}

void PlaneStress2dXfem :: giveInputRecord(DynamicInputRecord &input)
{
    PlaneStress2d :: giveInputRecord(input);
    XfemStructuralElementInterface :: giveCZInputRecord(input);
}

void
PlaneStress2dXfem :: giveCompositeExportData(std::vector< ExportRegion > &vtkPieces, IntArray &primaryVarsToExport, IntArray &internalVarsToExport, IntArray cellVarsToExport, TimeStep *tStep)
{
    vtkPieces.resize(1);

    const int numCells = mSubTri.size();

    if(numCells == 0) {
        // Enriched but uncut element
        // Visualize as a quad
        vtkPieces[0].setNumberOfCells(1);

        int numTotalNodes = 4;
        vtkPieces[0].setNumberOfNodes(numTotalNodes);

        // Node coordinates
        std :: vector< FloatArray >nodeCoords;
        for(int i = 1; i <= 4; i++) {
            const auto &x = giveDofManager(i)->giveCoordinates();
            nodeCoords.push_back(x);

            vtkPieces[0].setNodeCoords(i, x);
        }

        // Connectivity
        IntArray nodes1 = {1, 2, 3, 4};
        vtkPieces[0].setConnectivity(1, nodes1);

        // Offset
        int offset = 4;
        vtkPieces[0].setOffset(1, offset);

        // Cell types
        vtkPieces[0].setCellType(1, 9); // Linear quad




        // Export nodal variables from primary fields
        vtkPieces[0].setNumberOfPrimaryVarsToExport(primaryVarsToExport, numTotalNodes);

        for ( int fieldNum = 1; fieldNum <= primaryVarsToExport.giveSize(); fieldNum++ ) {
            UnknownType type = ( UnknownType ) primaryVarsToExport.at(fieldNum);

            for ( int nodeInd = 1; nodeInd <= numTotalNodes; nodeInd++ ) {

                if ( type == DisplacementVector ) { // compute displacement

                        FloatArray u = {0.0, 0.0, 0.0};

                        // Fetch global coordinates (in undeformed configuration)
                        const FloatArray &x = nodeCoords[nodeInd-1];

                        // Compute local coordinates
                        FloatArray locCoord;
                        computeLocalCoordinates(locCoord, x);

                        // Compute displacement in point
                        FloatMatrix NMatrix;
                        computeNmatrixAt(locCoord, NMatrix);
                        FloatArray solVec;
                        computeVectorOf(VM_Total, tStep, solVec);
                        FloatArray uTemp;
                        uTemp.beProductOf(NMatrix, solVec);

                        if(uTemp.giveSize() == 3) {
                            u = uTemp;
                        }
                        else {
                            u = {uTemp[0], uTemp[1], 0.0};
                        }

                        vtkPieces[0].setPrimaryVarInNode(type, nodeInd, u);
                } else {
                    printf("fieldNum: %d\n", fieldNum);
                    // TODO: Implement
//                    ZZNodalRecoveryMI_recoverValues(values, layer, ( InternalStateType ) 1, tStep); // does not work well - fix
//                    for ( int j = 1; j <= numCellNodes; j++ ) {
//                        vtkPiece.setPrimaryVarInNode(fieldNum, nodeNum, values [ j - 1 ]);
//                        nodeNum += 1;
//                    }
                }
            }
        }


        // Export nodal variables from internal fields
        vtkPieces[0].setNumberOfInternalVarsToExport(internalVarsToExport, numTotalNodes);


        // Export cell variables
        vtkPieces[0].setNumberOfCellVarsToExport(cellVarsToExport, 1);
        for ( int i = 1; i <= cellVarsToExport.giveSize(); i++ ) {
            InternalStateType type = ( InternalStateType ) cellVarsToExport.at(i);
            FloatArray average;
            std :: unique_ptr< IntegrationRule >&iRule = integrationRulesArray [ 0 ];
            VTKXMLExportModule :: computeIPAverage(average, iRule.get(), this, type, tStep);

            FloatArray averageVoigt;

            if( average.giveSize() == 6 ) {

                averageVoigt.resize(9);

                averageVoigt.at(1) = average.at(1);
                averageVoigt.at(5) = average.at(2);
                averageVoigt.at(9) = average.at(3);
                averageVoigt.at(6) = averageVoigt.at(8) = average.at(4);
                averageVoigt.at(3) = averageVoigt.at(7) = average.at(5);
                averageVoigt.at(2) = averageVoigt.at(4) = average.at(6);
            }
            else {
                if(average.giveSize() == 1) {
                    averageVoigt.resize(1);
                    averageVoigt.at(1) = average.at(1);
                }
            }

            vtkPieces[0].setCellVar( type, 1, averageVoigt );
        }


        // Export of XFEM related quantities
        if ( domain->hasXfemManager() ) {
            XfemManager *xMan = domain->giveXfemManager();

            int nEnrIt = xMan->giveNumberOfEnrichmentItems();
            vtkPieces[0].setNumberOfInternalXFEMVarsToExport(xMan->vtkExportFields.giveSize(), nEnrIt, numTotalNodes);

            const int nDofMan = giveNumberOfDofManagers();


            for ( int field = 1; field <= xMan->vtkExportFields.giveSize(); field++ ) {
                XFEMStateType xfemstype = ( XFEMStateType ) xMan->vtkExportFields [ field - 1 ];

                for ( int enrItIndex = 1; enrItIndex <= nEnrIt; enrItIndex++ ) {
                    EnrichmentItem *ei = xMan->giveEnrichmentItem(enrItIndex);
                    for ( int nodeInd = 1; nodeInd <= numTotalNodes; nodeInd++ ) {

                        const FloatArray &x = nodeCoords[nodeInd-1];
                        FloatArray locCoord;
                        computeLocalCoordinates(locCoord, x);

                        FloatArray N;
                        FEInterpolation *interp = giveInterpolation();
                        interp->evalN( N, locCoord, FEIElementGeometryWrapper(this) );


                        if ( xfemstype == XFEMST_LevelSetPhi ) {
                            double levelSet = 0.0, levelSetInNode = 0.0;

                            for(int elNodeInd = 1; elNodeInd <= nDofMan; elNodeInd++) {
                                DofManager *dMan = giveDofManager(elNodeInd);
                                ei->evalLevelSetNormalInNode(levelSetInNode, dMan->giveGlobalNumber(), dMan->giveCoordinates() );

                                levelSet += N.at(elNodeInd)*levelSetInNode;
                            }


                            FloatArray valueArray = {levelSet};
                            vtkPieces[0].setInternalXFEMVarInNode(field, enrItIndex, nodeInd, valueArray);

                        } else if ( xfemstype == XFEMST_LevelSetGamma ) {
                            double levelSet = 0.0, levelSetInNode = 0.0;

                            for(int elNodeInd = 1; elNodeInd <= nDofMan; elNodeInd++) {
                                DofManager *dMan = giveDofManager(elNodeInd);
                                ei->evalLevelSetTangInNode(levelSetInNode, dMan->giveGlobalNumber(), dMan->giveCoordinates() );

                                levelSet += N.at(elNodeInd)*levelSetInNode;
                            }


                            FloatArray valueArray = {levelSet};
                            vtkPieces[0].setInternalXFEMVarInNode(field, enrItIndex, nodeInd, valueArray);

                        } else if ( xfemstype == XFEMST_NodeEnrMarker ) {
                            double nodeEnrMarker = 0.0, nodeEnrMarkerInNode = 0.0;

                            for(int elNodeInd = 1; elNodeInd <= nDofMan; elNodeInd++) {
                                DofManager *dMan = giveDofManager(elNodeInd);
                                ei->evalNodeEnrMarkerInNode(nodeEnrMarkerInNode, dMan->giveGlobalNumber() );

                                nodeEnrMarker += N.at(elNodeInd)*nodeEnrMarkerInNode;
                            }


                            FloatArray valueArray = {nodeEnrMarker};
                            vtkPieces[0].setInternalXFEMVarInNode(field, enrItIndex, nodeInd, valueArray);
                        }

                    }
                }
            }
        }

    }
    else {
        // Enriched and cut element

        XfemStructuralElementInterface::giveSubtriangulationCompositeExportData(vtkPieces, primaryVarsToExport, internalVarsToExport, cellVarsToExport, tStep);


    }
}

} // end namespace oofem
