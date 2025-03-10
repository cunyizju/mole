/*
 * plmaterialforce.C
 *
 *  Created on: Nov 14, 2014
 *      Author: svennine
 */

#include "plmaterialforce.h"

#include "xfem/propagationlaw.h"
#include "xfem/tipinfo.h"
#include "engng/classfactory.h"
#include "math/mathfem.h"
#include "input/dynamicinputrecord.h"
#include "mesher/spatiallocalizer.h"
#include "math/floatmatrix.h"
#include "math/gausspoint.h"
#include "sm/Materials/Structural/structuralms.h"
#include "xfem/enrichmentitem.h"
#include "fei/feinterpol.h"
#include "xfem/xfemmanager.h"
#include "xfem/matforceevaluator.h"

#include "engng/engngm.h"

namespace oofem {
REGISTER_PropagationLaw(PLMaterialForce)

PLMaterialForce :: PLMaterialForce():
    mRadius(0.0),
    mIncrementLength(0.0),
    mCrackPropThreshold(0.0),
    mpMaterialForceEvaluator( new MaterialForceEvaluator() )
{}

PLMaterialForce :: ~PLMaterialForce()
{}

void PLMaterialForce :: initializeFrom(InputRecord &ir)
{
    IR_GIVE_FIELD(ir, mRadius,                          _IFT_PLMaterialForce_Radius);
    IR_GIVE_FIELD(ir, mIncrementLength,                 _IFT_PLMaterialForce_IncLength);
    IR_GIVE_OPTIONAL_FIELD(ir, mCrackPropThreshold,     _IFT_PLMaterialForce_CrackPropThreshold);
}

void PLMaterialForce :: giveInputRecord(DynamicInputRecord &input)
{
    int number = 1;
    input.setRecordKeywordField(this->giveInputRecordName(), number);

    input.setField(mRadius,                     _IFT_PLMaterialForce_Radius);
    input.setField(mIncrementLength,            _IFT_PLMaterialForce_IncLength);
    input.setField(mCrackPropThreshold,         _IFT_PLMaterialForce_CrackPropThreshold);
}

bool PLMaterialForce :: propagateInterface(Domain &iDomain, EnrichmentFront &iEnrFront, TipPropagation &oTipProp)
{
//    printf("Entering PLMaterialForce :: propagateInterface().\n");

    if ( !iEnrFront.propagationIsAllowed() ) {
        return false;
    }

    // Fetch crack tip data
    const TipInfo &tipInfo = iEnrFront.giveTipInfo();

    // Check if the tip is located in the domain
    SpatialLocalizer *localizer = iDomain.giveSpatialLocalizer();
    FloatArray lCoords, closest;
//    printf("tipInfo.mGlobalCoord: \n"); tipInfo.mGlobalCoord.printYourself();
    if ( tipInfo.mGlobalCoord.giveSize() == 0 ) {
        return false;
    }

    localizer->giveElementClosestToPoint(lCoords, closest, tipInfo.mGlobalCoord);

    if ( distance(closest, tipInfo.mGlobalCoord) > 1.0e-9 ) {
//        printf("Tip is outside all elements.\n");
        return false;
    }


    FloatArray matForce;
    TimeStep *tStep = iDomain.giveEngngModel()->giveCurrentStep();
    mpMaterialForceEvaluator->computeMaterialForce(matForce, iDomain, tipInfo, tStep, mRadius);

//    printf("matForce: "); matForce.printYourself();

    if(matForce.giveSize() == 0) {
        return false;
    }

    double forceNorm = matForce.computeNorm();
//    printf("forceNorm: %e mCrackPropThreshold: %e\n", forceNorm, mCrackPropThreshold);

    if(forceNorm < mCrackPropThreshold || forceNorm < 1.0e-20) {
        return false;
    }

    printf("forceNorm: %e mCrackPropThreshold: %e\n", forceNorm, mCrackPropThreshold);
    printf("Propagating crack in PLMaterialForce :: propagateInterface.\n");
//    printf("Tip coord: "); tipInfo.mGlobalCoord.printYourself();

    FloatArray dir(matForce);
    dir.times(1.0/forceNorm);
//    printf("dir: "); dir.printYourself();

    const double cosAngTol = 1.0/sqrt(2.0);
    if ( tipInfo.mTangDir.dotProduct(dir) < cosAngTol ) {
        // Do not allow sharper turns than 45 degrees

        if( tipInfo.mNormalDir.dotProduct(dir) > 0.0 ) {
            dir = tipInfo.mTangDir;
            dir.add(tipInfo.mNormalDir);
            dir.normalize();
        }
        else {
//            dir = tipInfo.mNormalDir;
//            dir.times(-1.0);
            dir = tipInfo.mTangDir;
            dir.add(-1.0,tipInfo.mNormalDir);
            dir.normalize();
        }

        printf("//////////////////////////////////////////// Resticting crack propagation direction.\n");
//        printf("tipInfo.mTangDir: "); tipInfo.mTangDir.printYourself();
//        printf("dir: "); dir.printYourself();
    }

    // Fill up struct
    oTipProp.mTipIndex = tipInfo.mTipIndex;
    oTipProp.mPropagationDir = dir;
    oTipProp.mPropagationLength = mIncrementLength;


    return true;
}

} /* namespace oofem */
