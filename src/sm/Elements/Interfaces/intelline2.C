// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Elements/Interfaces/intelline2.h"
#include "sm/CrossSections/structuralinterfacecrosssection.h"
#include "math/gausspoint.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrix.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "fei/fei2dlinequad.h"
#include "fei/fei2dlinelin.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_Element(IntElLine2);

FEI2dLineQuad IntElLine2 :: interp(2, 2);
FEI2dLineLin IntElLine2 :: interpLin(1, 1);


IntElLine2 :: IntElLine2(int n, Domain *aDomain) : IntElLine1(n, aDomain)
{
    numberOfDofMans = 6;
    numberOfGaussPoints = 4;
    linear = false;
}


void
IntElLine2 :: computeNmatrixAt(GaussPoint *ip, FloatMatrix &answer)
{
    // Returns the modified N-matrix which multiplied with u give the spatial jump.
    FloatArray N;
    answer.resize(2, 12);
    answer.zero();

    if ( linear ) {
        interpLin.evalN( N, ip->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );

        answer.at(1, 1) = answer.at(2, 2) = -N.at(1);
        answer.at(1, 3) = answer.at(2, 4) = -N.at(2);
        //answer.at(1, 5) = answer.at(2, 6) = -N.at(3);

        answer.at(1, 7) = answer.at(2, 8) = N.at(1);
        answer.at(1, 9) = answer.at(2, 10) = N.at(2);
        //answer.at(1, 11) = answer.at(2, 12) = N.at(3);
    } else {
        interp.evalN( N, ip->giveNaturalCoordinates(), FEIElementGeometryWrapper(this) );

        answer.at(1, 1) = answer.at(2, 2) = -N.at(1);
        answer.at(1, 3) = answer.at(2, 4) = -N.at(2);
        answer.at(1, 5) = answer.at(2, 6) = -N.at(3);

        answer.at(1, 7) = answer.at(2, 8) = N.at(1);
        answer.at(1, 9) = answer.at(2, 10) = N.at(2);
        answer.at(1, 11) = answer.at(2, 12) = N.at(3);
    }
}


void
IntElLine2 :: computeGaussPoints()
{
    if ( integrationRulesArray.size() == 0 ) {
        integrationRulesArray.resize( 1 );
        //integrationRulesArray[ 0 ] = std::make_unique<LobattoIntegrationRule>(1,domain, 1, 2); ///@todo - should be able to decide
        integrationRulesArray [ 0 ] = std::make_unique<GaussIntegrationRule>(1, this, 1, 2);
        integrationRulesArray [ 0 ]->SetUpPointsOnLine(numberOfGaussPoints, _2dInterface); ///@todo - should be a parameter with num of ip
    }
}

FEInterpolation *
IntElLine2 :: giveInterpolation() const
{
    return & interp;
}


void
IntElLine2 :: initializeFrom(InputRecord &ir)
{
    IntElLine1 :: initializeFrom(ir);
    linear = ir.hasField(_IFT_IntElLine2_LinearTraction);
}

} // end namespace oofem
