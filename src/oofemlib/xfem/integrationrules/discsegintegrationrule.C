// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "discsegintegrationrule.h"

#include "math/gausspoint.h"
#include "fei/fei1dlin.h"

namespace oofem {
DiscontinuousSegmentIntegrationRule :: DiscontinuousSegmentIntegrationRule(int n, Element *e, const std :: vector< Line > &iSegments) :
    GaussIntegrationRule(n, e),
    mSegments(iSegments)
{}

DiscontinuousSegmentIntegrationRule :: ~DiscontinuousSegmentIntegrationRule() {}

int DiscontinuousSegmentIntegrationRule :: SetUpPointsOnLine(int iNumPointsPerSeg, MaterialMode mode)
{
    int numPointsTot = iNumPointsPerSeg * mSegments.size();
    int pointsPassed = 0;

    ////////////////////////////////////////////
    // Allocate Gauss point array
    FloatArray coords_xi, weights;
    this->giveLineCoordsAndWeights(iNumPointsPerSeg, coords_xi, weights);
    this->gaussPoints.resize(numPointsTot);
    ////////////////////////////////////////////

    double totalLength = 0.0;
    for( Line &line : mSegments ) {
    	totalLength += line.giveLength();
    }

    const FloatArray &xS = mSegments[0].giveVertex(1);

    std :: vector< FloatArray >newGPCoord;

    // Loop over line segments
    for ( size_t i = 0; i < mSegments.size(); i++ ) {
        for ( int j = 0; j < iNumPointsPerSeg; j++ ) {
            FloatArray global;
            GaussPoint * &gp = this->gaussPoints [ pointsPassed ];

            gp = new GaussPoint(this, pointsPassed + 1, {coords_xi.at(j + 1)}, weights.at(j + 1), mode);

            const FloatArray &coord = gp->giveNaturalCoordinates();

            global.resize( xS.giveSize() );
            for ( int m = 1; m <= xS.giveSize(); m++ ) {

//            	if( mSegments [ i ].giveNrVertices() < 2 ) {
//            		printf("mSegments [ i ].giveNrVertices(): %d\n", mSegments [ i ].giveNrVertices() );
//            	}

                global.at(m) = 0.5 * ( ( 1.0 - coord.at(1) ) * mSegments [ i ].giveVertex(1).at(m) + ( 1.0 + coord.at(1) ) * mSegments [ i ].giveVertex(2).at(m) );
            }

            newGPCoord.push_back(global);


            // Local coordinate along the line segment
            double xi = 2.0 * ( distance(global, xS) / totalLength - 0.5 );
            gp->setNaturalCoordinates({ xi });

            gp->setSubPatchCoordinates({ xi });
            gp->setGlobalCoordinates(global);

            gp->setWeight(1.0 * gp->giveWeight() * mSegments [ i ].giveLength() / totalLength);  // update integration weight

            pointsPassed++;
        }
    }

    return this->giveNumberOfIntegrationPoints();
}
} /* namespace oofem */
