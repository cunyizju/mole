// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "enrichmentfrontintersection.h"
#include "input/dynamicinputrecord.h"
#include "engng/classfactory.h"
#include "xfem/xfemmanager.h"
#include "input/domain.h"
#include "input/connectivitytable.h"
#include "mesher/spatiallocalizer.h"
#include "input/element.h"
#include "math/gausspoint.h"

namespace oofem {
REGISTER_EnrichmentFront(EnrFrontIntersection)

EnrFrontIntersection :: EnrFrontIntersection() {}

EnrFrontIntersection :: ~EnrFrontIntersection() {}

void EnrFrontIntersection :: MarkNodesAsFront(std :: unordered_map< int, NodeEnrichmentType > &ioNodeEnrMarkerMap, XfemManager &ixFemMan,  const std :: unordered_map< int, double > &iLevelSetNormalDirMap, const std :: unordered_map< int, double > &iLevelSetTangDirMap, const TipInfo &iTipInfo)
{
    MarkTipElementNodesAsFront(ioNodeEnrMarkerMap, ixFemMan, iLevelSetNormalDirMap, iLevelSetTangDirMap, iTipInfo);
}

int EnrFrontIntersection :: giveNumEnrichments(const DofManager &iDMan) const
{
    return 1;
}

void EnrFrontIntersection :: evaluateEnrFuncAt(std :: vector< double > &oEnrFunc, const EfInput &iEfInput) const
{
    FloatArray xTip = {
        mTipInfo.mGlobalCoord.at(1), mTipInfo.mGlobalCoord.at(2)
    };

    FloatArray pos = {
        iEfInput.mPos.at(1), iEfInput.mPos.at(2)
    };

    // Crack tip normal and use defined tangent
    // Note that mTangent is not necessarily equal to mTipInfo.mTangDir!
    const FloatArray &t = mTangent;
    const FloatArray &n = mTipInfo.mNormalDir;

    FloatArray tipToPos = {
        iEfInput.mPos(0) - xTip(0), iEfInput.mPos(1) - xTip(1)
    };

    // Heaviside in normal direction
    double Hn = 0.0;
    if ( tipToPos.dotProduct(n) > 0.0 ) {
        Hn = 1.0;
    }

    // Heaviside in tangential direction
    double Ht = 0.0;
    if ( tipToPos.dotProduct(t) < 0.0 ) {
        Ht = 1.0;
    }

    oEnrFunc.push_back(Hn * Ht);
}

void EnrFrontIntersection :: evaluateEnrFuncDerivAt(std :: vector< FloatArray > &oEnrFuncDeriv, const EfInput &iEfInput, const FloatArray &iGradLevelSet) const
{
    FloatArray enrFuncDeriv = {
        0.0, 0.0
    };
    oEnrFuncDeriv.push_back(enrFuncDeriv);
}

void EnrFrontIntersection :: evaluateEnrFuncJumps(std :: vector< double > &oEnrFuncJumps, GaussPoint &iGP, int iNodeInd, bool iGPLivesOnCurrentCrack, const double &iNormalSignDist) const
{
    std :: vector< double >jumps;

    if ( iGPLivesOnCurrentCrack ) {
        jumps.push_back(1.0);
    } else   {
        //        printf("iNormalSignDist: %e\n", iNormalSignDist );

        if ( iNormalSignDist > 0.0 ) {
            jumps.push_back(1.0);
        } else   {
            jumps.push_back(0.0);
        }
    }

    oEnrFuncJumps.insert( oEnrFuncJumps.end(), jumps.begin(), jumps.end() );
}

void EnrFrontIntersection :: initializeFrom(InputRecord &ir)
{
    IR_GIVE_FIELD(ir, mTangent, _IFT_EnrFrontIntersection_Tangent);
}

void EnrFrontIntersection :: giveInputRecord(DynamicInputRecord &input)
{
    int number = 1;
    input.setRecordKeywordField(this->giveInputRecordName(), number);

    input.setField(mTangent, _IFT_EnrFrontIntersection_Tangent);
}
} /* namespace oofem */
