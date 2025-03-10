// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "enrichmentfrontlinbranchfunconeel.h"
#include "input/dynamicinputrecord.h"
#include "engng/classfactory.h"
#include "xfem/xfemmanager.h"
#include "input/domain.h"
#include "mesher/spatiallocalizer.h"
#include "input/element.h"
#include "math/gausspoint.h"
#include "math/mathfem.h"

namespace oofem {
REGISTER_EnrichmentFront(EnrFrontLinearBranchFuncOneEl)


EnrFrontLinearBranchFuncOneEl :: EnrFrontLinearBranchFuncOneEl() :
    mpBranchFunc()
{ }

EnrFrontLinearBranchFuncOneEl :: ~EnrFrontLinearBranchFuncOneEl() { }


void EnrFrontLinearBranchFuncOneEl :: MarkNodesAsFront(std :: unordered_map< int, NodeEnrichmentType > &ioNodeEnrMarkerMap, XfemManager &ixFemMan,  const std :: unordered_map< int, double > &iLevelSetNormalDirMap, const std :: unordered_map< int, double > &iLevelSetTangDirMap, const TipInfo &iTipInfo)
{
    MarkTipElementNodesAsFront(ioNodeEnrMarkerMap, ixFemMan, iLevelSetNormalDirMap, iLevelSetTangDirMap, iTipInfo);
}

int EnrFrontLinearBranchFuncOneEl :: giveNumEnrichments(const DofManager &iDMan) const
{
    return 4;
}

void EnrFrontLinearBranchFuncOneEl :: evaluateEnrFuncAt(std :: vector< double > &oEnrFunc, const EfInput &iEfInput) const
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

#ifdef DEBUG
    for ( double val:oEnrFunc ) {
        if ( !std :: isfinite(val) ) {
            printf("r: %e theta: %e\n", r, theta);
            OOFEM_ERROR("!std::isfinite(val)")
        }
    }
#endif
}

void EnrFrontLinearBranchFuncOneEl :: evaluateEnrFuncDerivAt(std :: vector< FloatArray > &oEnrFuncDeriv, const EfInput &iEfInput, const FloatArray &iGradLevelSet) const
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

void EnrFrontLinearBranchFuncOneEl :: evaluateEnrFuncJumps(std :: vector< double > &oEnrFuncJumps, GaussPoint &iGP, int iNodeInd, bool iGPLivesOnCurrentCrack, const double &iNormalSignDist) const
{
    const FloatArray &xTip = mTipInfo.mGlobalCoord;
    const FloatArray &gpCoord = iGP.giveGlobalCoordinates();

    double radius = distance(gpCoord, xTip);

    std :: vector< double >jumps;
    mpBranchFunc.giveJump(jumps, radius);

    oEnrFuncJumps.insert( oEnrFuncJumps.end(), jumps.begin(), jumps.end() );
}

void EnrFrontLinearBranchFuncOneEl :: initializeFrom(InputRecord &ir)
{
}

void EnrFrontLinearBranchFuncOneEl :: giveInputRecord(DynamicInputRecord &input)
{
    int number = 1;
    input.setRecordKeywordField(this->giveInputRecordName(), number);
}
} // end namespace oofem
