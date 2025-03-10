// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li


#include "enrichmentfrontlinbranchfuncradius.h"
#include "input/dynamicinputrecord.h"
#include "engng/classfactory.h"
#include "xfem/xfemmanager.h"
#include "input/domain.h"
#include "input/connectivitytable.h"
#include "math/gausspoint.h"

namespace oofem {
REGISTER_EnrichmentFront(EnrFrontLinearBranchFuncRadius)

EnrFrontLinearBranchFuncRadius :: EnrFrontLinearBranchFuncRadius() :
    mEnrichmentRadius(0.0),
    mpBranchFunc()
{ }

EnrFrontLinearBranchFuncRadius :: ~EnrFrontLinearBranchFuncRadius() { }

void EnrFrontLinearBranchFuncRadius :: MarkNodesAsFront(std :: unordered_map< int, NodeEnrichmentType > &ioNodeEnrMarkerMap, XfemManager &ixFemMan, const std :: unordered_map< int, double > &iLevelSetNormalDirMap, const std :: unordered_map< int, double > &iLevelSetTangDirMap, const TipInfo &iTipInfo)
{
    // Enrich all nodes within a prescribed radius around the crack tips.
    // TODO: If performance turns out to be an issue, we may wish
    // to put the nodes in a Kd tree (or similar) to speed up searching.
    // For now, loop over all nodes.

    mTipInfo = iTipInfo;

    Domain *d = ixFemMan.giveDomain();
    int nNodes = d->giveNumberOfDofManagers();

    // Make sure that the tip element gets enriched,
    // even if the radius is smaller than the element size
    MarkTipElementNodesAsFront(ioNodeEnrMarkerMap, ixFemMan, iLevelSetNormalDirMap, iLevelSetTangDirMap, iTipInfo);

    for ( int i = 1; i <= nNodes; i++ ) {
        DofManager *dMan = d->giveDofManager(i);
        const auto &nodePos = dMan->giveCoordinates();

        double radius2 = distance_square(iTipInfo.mGlobalCoord, nodePos);

        if ( radius2 < mEnrichmentRadius * mEnrichmentRadius ) {
            if ( ( ioNodeEnrMarkerMap [ i ] == NodeEnr_START_TIP && iTipInfo.mTipIndex == 1 ) ||
                 ( ioNodeEnrMarkerMap [ i ] == NodeEnr_END_TIP && iTipInfo.mTipIndex == 0 ) ) {
                ioNodeEnrMarkerMap [ i ] = NodeEnr_START_AND_END_TIP;
            } else   {
                if ( iTipInfo.mTipIndex == 0 ) {
                    ioNodeEnrMarkerMap [ i ] = NodeEnr_START_TIP;
                }

                if ( iTipInfo.mTipIndex == 1 ) {
                    ioNodeEnrMarkerMap [ i ] = NodeEnr_END_TIP;
                }
            }
        }
    }
}

int EnrFrontLinearBranchFuncRadius :: giveNumEnrichments(const DofManager &iDMan) const
{
    return 4;
}

void EnrFrontLinearBranchFuncRadius :: evaluateEnrFuncAt(std :: vector< double > &oEnrFunc, const EfInput &iEfInput) const
{
    FloatArray xTip = {
        mTipInfo.mGlobalCoord.at(1), mTipInfo.mGlobalCoord.at(2)
    };

    FloatArray pos = {
        iEfInput.mPos.at(1), iEfInput.mPos.at(2)
    };

    // Crack tangent and normal
    FloatArray t, n;
    bool flipTangent = false;
    computeCrackTangent(t, n, flipTangent, iEfInput);

    double r = 0.0, theta = 0.0;
    EnrichmentItem :: calcPolarCoord(r, theta, xTip, pos, n, t, iEfInput, flipTangent);

    mpBranchFunc.evaluateEnrFuncAt(oEnrFunc, r, theta);
}

void EnrFrontLinearBranchFuncRadius :: evaluateEnrFuncDerivAt(std :: vector< FloatArray > &oEnrFuncDeriv, const EfInput &iEfInput, const FloatArray &iGradLevelSet) const
{
    const FloatArray &xTip = mTipInfo.mGlobalCoord;

    // Crack tangent and normal
    FloatArray t, n;
    bool flipTangent = false;
    computeCrackTangent(t, n, flipTangent, iEfInput);

    double r = 0.0, theta = 0.0;
    EnrichmentItem :: calcPolarCoord(r, theta, xTip, iEfInput.mPos, n, t, iEfInput, flipTangent);


    size_t sizeStart = oEnrFuncDeriv.size();
    mpBranchFunc.evaluateEnrFuncDerivAt(oEnrFuncDeriv, r, theta);

    /**
     * Transform to global coordinates.
     */
    FloatMatrix E;
    E.resize(2, 2);
    E.setColumn(t, 1);
    E.setColumn(n, 2);


    for ( size_t j = sizeStart; j < oEnrFuncDeriv.size(); j++ ) {
        FloatArray enrFuncDerivGlob;
        enrFuncDerivGlob.beProductOf(E, oEnrFuncDeriv [ j ]);
        oEnrFuncDeriv [ j ] = enrFuncDerivGlob;
    }
}

void EnrFrontLinearBranchFuncRadius :: evaluateEnrFuncJumps(std :: vector< double > &oEnrFuncJumps, GaussPoint &iGP, int iNodeInd, bool iGPLivesOnCurrentCrack, const double &iNormalSignDist) const
{
    const FloatArray &xTip = mTipInfo.mGlobalCoord;
    const FloatArray &gpCoord = iGP.giveGlobalCoordinates();
    double radius = distance(gpCoord, xTip);

    std :: vector< double >jumps;
    mpBranchFunc.giveJump(jumps, radius);

    oEnrFuncJumps.insert( oEnrFuncJumps.end(), jumps.begin(), jumps.end() );
}

void EnrFrontLinearBranchFuncRadius :: initializeFrom(InputRecord &ir)
{
    IR_GIVE_FIELD(ir, mEnrichmentRadius, _IFT_EnrFrontLinearBranchFuncRadius_Radius);
}

void EnrFrontLinearBranchFuncRadius :: giveInputRecord(DynamicInputRecord &input)
{
    int number = 1;
    input.setRecordKeywordField(this->giveInputRecordName(), number);

    input.setField(mEnrichmentRadius, _IFT_EnrFrontLinearBranchFuncRadius_Radius);
}
} // end namespace oofem
