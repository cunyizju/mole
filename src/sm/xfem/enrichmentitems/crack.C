// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li
#include "crack.h"
#include "engng/classfactory.h"
#include "sm/Materials/Interface/structuralinterfacematerialstatus.h"
#include "sm/export/gnuplotexportmodule.h"
#include "math/gausspoint.h"
#include "input/geometry.h"

#include "engng/engngm.h"
#include "sm/Materials/Multiscale/structuralfe2material.h"

namespace oofem {
REGISTER_EnrichmentItem(Crack)

Crack :: Crack(int n, XfemManager *xm, Domain *aDomain) : HybridEI(n, xm, aDomain)
{
    mpEnrichesDofsWithIdArray = {
        D_u, D_v, D_w
    };
}

void Crack :: initializeFrom(InputRecord &ir)
{
    EnrichmentItem :: initializeFrom(ir);
}

void Crack :: AppendCohesiveZoneGaussPoint(GaussPoint *ipGP)
{
    StructuralInterfaceMaterialStatus *matStat = dynamic_cast< StructuralInterfaceMaterialStatus * >( ipGP->giveMaterialStatus() );

    // Check that the material status is of an allowed type.
    if(matStat == NULL) {
    	StructuralFE2MaterialStatus *fe2ms = dynamic_cast<StructuralFE2MaterialStatus*> ( ipGP->giveMaterialStatus() );

    	if(fe2ms == NULL) {
    		OOFEM_ERROR("The material status is not of an allowed type.")
    	}
    }

//    if ( matStat ) {
        // Compute arc length position of the Gauss point
        const FloatArray &coord =  ipGP->giveGlobalCoordinates();
        double tangDist = 0.0, arcPos = 0.0;
        mpBasicGeometry->computeTangentialSignDist(tangDist, coord, arcPos);

        // Insert at correct position
        std :: vector< GaussPoint * > :: iterator iteratorGP = mCohesiveZoneGaussPoints.begin();
        std :: vector< double > :: iterator iteratorPos = mCohesiveZoneArcPositions.begin();
        for ( size_t i = 0; i < mCohesiveZoneArcPositions.size(); i++ ) {
            if ( arcPos > mCohesiveZoneArcPositions [ i ] ) {
                iteratorGP++;
                iteratorPos++;
            }
        }

        mCohesiveZoneGaussPoints.insert(iteratorGP, ipGP);
        mCohesiveZoneArcPositions.insert(iteratorPos, arcPos);
//    } else {
//        OOFEM_ERROR("matStat == NULL.")
//    }
}

void Crack :: callGnuplotExportModule(GnuplotExportModule &iExpMod, TimeStep *tStep)
{
    iExpMod.outputXFEM(* this, tStep);
}

void Crack :: computeCrackIntersectionPoints(Crack &iCrack, std :: vector< FloatArray > &oIntersectionPoints, std :: vector< double > &oArcPositions)
{
    const double tol = 1.0e-12;

    // Enrichment domain of the current crack
    PolygonLine *polygonLine1 = dynamic_cast< PolygonLine * >( mpBasicGeometry.get() );

    // Enrichment domain of the crack given as input
    PolygonLine *polygonLine2 = dynamic_cast< PolygonLine * >( iCrack.giveGeometry() );

    if ( polygonLine1 != NULL && polygonLine2 != NULL ) {
        polygonLine2->computeIntersectionPoints(* polygonLine1, oIntersectionPoints);

        for ( FloatArray pos:oIntersectionPoints ) {
            double tangDist, arcPos;
            polygonLine1->computeTangentialSignDist(tangDist, pos, arcPos);

            if ( arcPos < -tol || arcPos > ( 1.0 + tol ) ) {
                printf("arcPos: %e\n", arcPos);
                OOFEM_ERROR("arcPos is outside the allowed range [0,1].")
            }

            oArcPositions.push_back(arcPos);
        }
    }
}

void Crack :: computeArcPoints(const std :: vector< FloatArray > &iIntersectionPoints, std :: vector< double > &oArcPositions)
{
    const double tol = 1.0e-12;

    // Enrichment domain of the current crack
    PolygonLine *polygonLine1 = dynamic_cast< PolygonLine * >( mpBasicGeometry.get() );

    if ( polygonLine1 != NULL ) {
        for ( FloatArray pos:iIntersectionPoints ) {
            double tangDist, arcPos;
            polygonLine1->computeTangentialSignDist(tangDist, pos, arcPos);

            if ( arcPos < -tol || arcPos > ( 1.0 + tol ) ) {
                printf("arcPos: %e\n", arcPos);
                OOFEM_ERROR("arcPos is outside the allowed range [0,1].")
            }

            oArcPositions.push_back(arcPos);
        }
    }
}

double Crack :: computeLength()
{
    PolygonLine *polygonLine = dynamic_cast< PolygonLine * >( mpBasicGeometry.get() );

    if ( polygonLine != NULL ) {
        return polygonLine->computeLength();
    }

    return 0.0;
}

int Crack :: giveDofPoolSize() const
{
    return this->giveEnrichesDofsWithIdArray()->giveSize() * this->giveNumberOfEnrDofs() + 5;
}
} // end namespace oofem
