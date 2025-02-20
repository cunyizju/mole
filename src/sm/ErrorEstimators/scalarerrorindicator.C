// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/ErrorEstimators/scalarerrorindicator.h"
#include "sm/ErrorEstimators/directerrorindicatorrc.h"
#include "input/element.h"
#include "math/integrationrule.h"
#include "math/gausspoint.h"
#include "math/mathfem.h"
#include "error/errorestimatortype.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_ErrorEstimator(ScalarErrorIndicator, EET_SEI);

int
ScalarErrorIndicator :: estimateError(EE_ErrorMode mode, TimeStep *tStep)
{
    if ( indicatorType == 1 ) {
        if ( mode == equilibratedEM ) {
            varType = IST_PrincipalDamageTensor;
        } else {
            varType = IST_PrincipalDamageTempTensor;
        }
    }

    return 1;
}

double
ScalarErrorIndicator :: giveElementError(EE_ErrorType type, Element *elem, TimeStep *tStep)
{
    FloatArray val;
    int result = 1;
    double sval, maxVal = 0.0;

    if ( type != indicatorET ) {
        return 0.0;
    }

    if ( this->skipRegion( elem->giveRegionNumber() ) ) {
        return 0.0;
    }

    for ( GaussPoint *gp: *elem->giveDefaultIntegrationRulePtr() ) {
        result = elem->giveIPValue(val, gp, varType, tStep);
        if ( result ) {
            sval = val.computeNorm();
            if ( gp->giveNumber() == 1 ) {
                maxVal = sval;
            } else {
                maxVal = max(maxVal, sval);
            }
        }
    }

    return maxVal;
}


void
ScalarErrorIndicator :: initializeFrom(InputRecord &ir)
{
    ErrorEstimator :: initializeFrom(ir);

    IR_GIVE_FIELD(ir, indicatorType, _IFT_ScalarErrorIndicator_vartype);
    if ( indicatorType != 1 ) {
        OOFEM_ERROR("usupported varType");
    }

    return this->giveRemeshingCrit()->initializeFrom(ir);
}

RemeshingCriteria *
ScalarErrorIndicator :: giveRemeshingCrit()
{
    if ( !this->rc ) {
        this->rc = std::make_unique<DirectErrorIndicatorRC>(1, this);
    }

    return this->rc.get();
}
} // end namespace oofem
