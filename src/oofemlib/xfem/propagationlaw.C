// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li



#include "propagationlaw.h"
#include "tipinfo.h"
#include "engng/classfactory.h"
#include "math/mathfem.h"
#include "input/dynamicinputrecord.h"
#include "mesher/spatiallocalizer.h"
#include "math/floatmatrix.h"
#include "math/gausspoint.h"
#include "enrichmentitem.h"
#include "fei/feinterpol.h"
#include "xfemmanager.h"

#include "XFEMDebugTools.h"

namespace oofem {
REGISTER_PropagationLaw(PLDoNothing)
REGISTER_PropagationLaw(PLCrackPrescribedDir)
REGISTER_PropagationLaw(PLnodeRadius)

PropagationLaw :: PropagationLaw() { }

PropagationLaw :: ~PropagationLaw() { }

void PLDoNothing :: giveInputRecord(DynamicInputRecord &input)
{
    int number = 1;
    input.setRecordKeywordField(this->giveInputRecordName(), number);
}

void PLCrackPrescribedDir :: initializeFrom(InputRecord &ir)
{
    IR_GIVE_FIELD(ir, mAngle, _IFT_PLCrackPrescribedDir_Dir);
    IR_GIVE_FIELD(ir, mIncrementLength, _IFT_PLCrackPrescribedDir_IncLength);

    //    printf("In PLCrackPrescribedDir :: initializeFrom: mAngle: %e mIncrementLength: %e\n", mAngle, mIncrementLength);
}

void PLCrackPrescribedDir :: giveInputRecord(DynamicInputRecord &input)
{
    int number = 1;
    input.setRecordKeywordField(this->giveInputRecordName(), number);

    input.setField(mAngle, _IFT_PLCrackPrescribedDir_Dir);
    input.setField(mIncrementLength, _IFT_PLCrackPrescribedDir_IncLength);
}

bool PLCrackPrescribedDir :: propagateInterface(Domain &iDomain, EnrichmentFront &iEnrFront, TipPropagation &oTipProp)
{
    if ( !iEnrFront.propagationIsAllowed() ) {
        return false;
    }

    const TipInfo &tipInfo = iEnrFront.giveTipInfo();

    SpatialLocalizer *localizer = iDomain.giveSpatialLocalizer();
    // It is meaningless to propagate a tip that is not inside any element
    if ( tipInfo.mGlobalCoord.giveSize() == 0 ) {
        return false;
    }

    Element *el = localizer->giveElementContainingPoint(tipInfo.mGlobalCoord);
    if ( el == NULL ) {
        return false;
    }


    double angleRad = mAngle * M_PI / 180.0;
    FloatArray dir = {
        cos(angleRad), sin(angleRad)
    };

    oTipProp.mTipIndex = tipInfo.mTipIndex;
    oTipProp.mPropagationDir = dir;
    oTipProp.mPropagationLength = mIncrementLength;

    return true;
}

void PLnodeRadius :: initializeFrom(InputRecord &ir)
{
    IR_GIVE_FIELD(ir, mRadius, _IFT_PLnodeRadius_Radius);

    //printf("In PLnodeRadius :: initializeFrom: mRadius: %e \n", mRadius);
}

void PLnodeRadius :: giveInputRecord(DynamicInputRecord &input)
{
    int number = 1;
    input.setRecordKeywordField(this->giveInputRecordName(), number);

    input.setField(mRadius, _IFT_PLnodeRadius_Radius);
}

bool PLnodeRadius :: propagateInterface(Domain &iDomain, EnrichmentFront &iEnrFront, TipPropagation &oTipProp)
{
    if ( !iEnrFront.propagationIsAllowed() ) {
        printf("EnrichmentFront.propagationIsAllowed is false \n");
        return false;
    }
    
    const TipInfo &tipInfo = iEnrFront.giveTipInfo();   // includes the dofman numbers which represent the boundary of the EI. 
    //tipInfo.mTipDofManNumbers.printYourself();
        
    // No listbased tip (or EI) present, so nothing to propagate. 
    if ( tipInfo.mTipDofManNumbers.giveSize() == 0 ) {
        printf("No dofmans in tip; nothing to propagate. \n");
        return false;  
    }
    
    // Localise nodes within certain radius from tip nodes
    oTipProp.mPropagationDofManNumbers.clear();
    SpatialLocalizer *localizer = iDomain.giveSpatialLocalizer();
    
    for ( int i = 1 ; i <= tipInfo.mTipDofManNumbers.giveSize() ; i++ ) {
        
        //DofManager *dofMan  = iDomain.giveDofManager(tipInfo.mTipDofManNumbers.at(i));
        //const FloatArray gCoords = dofMan->giveCoordinates();
        Node *iNode = iDomain.giveNode(tipInfo.mTipDofManNumbers.at(i));
        const auto &gCoords = iNode->giveCoordinates();
        
        std :: list< int > nodeList;
        localizer->giveAllNodesWithinBox(nodeList,gCoords,mRadius);
        for ( int jNode : nodeList ) {
            //printf("nodeList node %d \n",jNode);
            oTipProp.mPropagationDofManNumbers.insertSortedOnce(jNode);
        }
        
    }
    //oTipProp.mPropagationDofManNumbers.printYourself(" The following noded will be propagated to:");
    
    return true;
}

} // end namespace oofem
