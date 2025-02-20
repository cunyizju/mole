// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "xfem/hybridei.h"
#include "xfemmanager.h"
#include "dofman/node.h"
#include "input/domain.h"
#include "math/floatmatrix.h"
#include "engng/classfactory.h"

#include <string>


namespace oofem {
REGISTER_EnrichmentItem(HybridEI)

HybridEI :: HybridEI(int n, XfemManager *xm, Domain *aDomain) :
    GeometryBasedEI(n, xm, aDomain)
{}

HybridEI :: ~HybridEI()
{}

void HybridEI :: evalLevelSetNormal(double &oLevelSet, const FloatArray &iGlobalCoord, const FloatArray &iN, const IntArray &iNodeInd) const
{
    interpLevelSet(oLevelSet, iN, iNodeInd);
}

void HybridEI :: evalLevelSetTangential(double &oLevelSet, const FloatArray &iGlobalCoord, const FloatArray &iN, const IntArray &iNodeInd) const
{
    interpLevelSetTangential(oLevelSet, iN, iNodeInd);
}

void HybridEI :: evalGradLevelSetNormal(FloatArray &oGradLevelSet, const FloatArray &iGlobalCoord, const FloatMatrix &idNdX, const IntArray &iNodeInd) const
{
    interpGradLevelSet(oGradLevelSet, idNdX, iNodeInd);
}

void HybridEI :: interpLevelSet(double &oLevelSet, const FloatArray &iN, const IntArray &iNodeInd) const
{
    oLevelSet = 0.0;
    for ( int i = 1; i <= iN.giveSize(); i++ ) {
        double levelSetNode = 0.0;
        const auto &nodePos = this->giveDomain()->giveNode(iNodeInd [ i - 1 ])->giveCoordinates();
        if ( evalLevelSetNormalInNode(levelSetNode, iNodeInd [ i - 1 ], nodePos) ) {
            oLevelSet += iN.at(i) * levelSetNode;
        }
    }
}

void HybridEI :: interpLevelSetTangential(double &oLevelSet, const FloatArray &iN, const IntArray &iNodeInd) const
{
    oLevelSet = 0.0;
    for ( int i = 1; i <= iN.giveSize(); i++ ) {
        double levelSetNode = 0.0;
        const auto &nodePos = this->giveDomain()->giveNode(iNodeInd [ i - 1 ])->giveCoordinates();
        if ( evalLevelSetTangInNode(levelSetNode, iNodeInd [ i - 1 ], nodePos) ) {
            oLevelSet += iN.at(i) * levelSetNode;
        }
    }
}

void HybridEI :: interpGradLevelSet(FloatArray &oGradLevelSet, const FloatMatrix &idNdX, const IntArray &iNodeInd) const
{
    int dim = idNdX.giveNumberOfColumns();

    if ( oGradLevelSet.giveSize() != dim ) {
        oGradLevelSet.resize(dim);
    }

    oGradLevelSet.zero();

    for ( int i = 1; i <= idNdX.giveNumberOfRows(); i++ ) {
        for ( int j = 1; j <= dim; j++ ) {
            double levelSetNode = 0.0;
            const auto &nodePos = this->giveDomain()->giveNode(iNodeInd [ i - 1 ])->giveCoordinates();
            if ( evalLevelSetNormalInNode(levelSetNode, iNodeInd [ i - 1 ], nodePos) ) {
                oGradLevelSet.at(j) += idNdX.at(i, j) * levelSetNode;
            }
        }
    }
}
} /* namespace oofem */
