// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/ErrorEstimators/combinedzzsiee.h"
#include "input/domain.h"
#include "input/element.h"
#include "input/connectivitytable.h"
#include "math/mathfem.h"
#include "error/errorestimatortype.h"
#include "engng/classfactory.h"

namespace oofem {
#define CZZSI_ZERO_INDICATOR_TOL 1.e-3

REGISTER_ErrorEstimator(CombinedZZSIErrorEstimator, EET_CZZSI);

void
CombinedZZSIErrorEstimator :: setDomain(Domain *d)
{
    FEMComponent :: setDomain(d);
    zzee.setDomain(d);
    siee.setDomain(d);
    this->giveRemeshingCrit()->setDomain(d);
}


int
CombinedZZSIErrorEstimator :: estimateError(EE_ErrorMode mode, TimeStep *tStep)
{
    int result = zzee.estimateError(mode, tStep);
    result += siee.estimateError(mode, tStep);
    return result;
}

double
CombinedZZSIErrorEstimator :: giveElementError(EE_ErrorType type, Element *elem, TimeStep *tStep)
{
    this->estimateError(equilibratedEM, tStep);
    if ( type == indicatorET ) {
        return siee.giveElementError(type, elem, tStep);
    } else {
        return zzee.giveElementError(type, elem, tStep);
    }
}

double
CombinedZZSIErrorEstimator :: giveValue(EE_ValueType type, TimeStep *tStep)
{
    return zzee.giveValue(type, tStep);
}

RemeshingCriteria *
CombinedZZSIErrorEstimator :: giveRemeshingCrit()
{
    if ( !this->rc ) {
        this->rc = std::make_unique<CombinedZZSIRemeshingCriteria>(1, this);
    }

    return this->rc.get();
}

void
CombinedZZSIErrorEstimator :: initializeFrom(InputRecord &ir)
{
    zzee.initializeFrom(ir);
    siee.initializeFrom(ir);

    return this->giveRemeshingCrit()->initializeFrom(ir);
}




CombinedZZSIRemeshingCriteria :: CombinedZZSIRemeshingCriteria(int n, ErrorEstimator *e) : RemeshingCriteria(n, e),
    zzrc(n, e), dirc(n, e)
{ }

double
CombinedZZSIRemeshingCriteria :: giveRequiredDofManDensity(int num, TimeStep *tStep, int relative)
{
    double indicatorVal, currDensity;
    double proposedDensity;
    this->estimateMeshDensities(tStep);

    dirc.giveNodeChar(num, tStep, indicatorVal, currDensity);
    if ( indicatorVal > dirc.giveMinIndicatorLimit() ) {
        return dirc.giveRequiredDofManDensity(num, tStep, relative);
    } else if ( fabs(indicatorVal) > CZZSI_ZERO_INDICATOR_TOL ) {
        //return zzrc.giveRequiredDofManDensity (num, tStep, relative);
        // transition between dirc and zzrc
        proposedDensity = zzrc.giveRequiredDofManDensity(num, tStep, relative);
        proposedDensity = min(proposedDensity, currDensity);
        proposedDensity = max( proposedDensity, dirc.giveMinIndicatorDensity() );
        if ( relative ) {
            return proposedDensity / currDensity;
        } else {
            return proposedDensity;
        }
    } else {
        return zzrc.giveRequiredDofManDensity(num, tStep, relative);
    }
}


RemeshingStrategy
CombinedZZSIRemeshingCriteria :: giveRemeshingStrategy(TimeStep *tStep)
{
    RemeshingStrategy s1, s2;
    this->estimateMeshDensities(tStep);

    s1 = zzrc.giveRemeshingStrategy(tStep);
    s2 = dirc.giveRemeshingStrategy(tStep);

    //if ((s1 == RemeshingFromPreviousState_RS) || (s2 == RemeshingFromPreviousState_RS)) return RemeshingFromPreviousState_RS;
    if ( ( s1 == RemeshingFromPreviousState_RS ) || ( s2 == RemeshingFromPreviousState_RS ) ) {
        return RemeshingFromCurrentState_RS;
    } else if ( ( s1 == RemeshingFromCurrentState_RS ) || ( s2 == RemeshingFromCurrentState_RS ) ) {
        return RemeshingFromCurrentState_RS;
    } else {
        return NoRemeshing_RS;
    }
}

int
CombinedZZSIRemeshingCriteria :: estimateMeshDensities(TimeStep *tStep)
{
    zzrc.estimateMeshDensities(tStep);
    dirc.estimateMeshDensities(tStep);
    return 1;
}

void
CombinedZZSIRemeshingCriteria :: initializeFrom(InputRecord &ir)
{
    zzrc.initializeFrom(ir);
    dirc.initializeFrom(ir);
}


double
CombinedZZSIRemeshingCriteria :: giveDofManDensity(int num)
{
    int isize;
    ConnectivityTable *ct = domain->giveConnectivityTable();
    const IntArray *con;
    double density = 0.0;

    con = ct->giveDofManConnectivityArray(num);
    isize = con->giveSize();

    for ( int i = 1; i <= isize; i++ ) {
        Element *element = domain->giveElement( con->at(i) );

        if ( i == 1 ) {
            density = element->computeMeanSize();
        } else {
            density = min( density, element->computeMeanSize() );
        }
    }

    return density;
}


void
CombinedZZSIRemeshingCriteria :: setDomain(Domain *d)
{
    FEMComponent :: setDomain(d);
    zzrc.setDomain(d);
    dirc.setDomain(d);
}
} // end namespace oofem
