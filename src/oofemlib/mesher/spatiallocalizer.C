// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "mesher/spatiallocalizer.h"
#include "input/connectivitytable.h"
#include "input/element.h"
#include "dofman/node.h"
#include "math/mathfem.h"
#include "error/error.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "fei/feinterpol.h"

namespace oofem {

int
SpatialLocalizerInterface :: SpatialLocalizerI_containsPoint(const FloatArray &coords)
{
    FloatArray lcoords;
    return this->element->computeLocalCoordinates(lcoords, coords);
}

void
SpatialLocalizerInterface :: SpatialLocalizerI_giveBBox(FloatArray &bb0, FloatArray &bb1)
{
    bb1 = bb0 = element->giveNode(1)->giveCoordinates();

    for ( int i = 2; i <= element->giveNumberOfNodes(); ++i ) {
        const auto &coordinates = element->giveNode(i)->giveCoordinates();
        bb0.beMinOf(bb0, coordinates);
        bb1.beMaxOf(bb1, coordinates);
    }
}


double
SpatialLocalizerInterface :: SpatialLocalizerI_giveClosestPoint(FloatArray &lcoords, FloatArray &closest, const FloatArray &gcoords)
{
    FEInterpolation *interp = element->giveInterpolation();

    if ( !interp->global2local( lcoords, gcoords, FEIElementGeometryWrapper(element) ) ) { // Outside element
        interp->local2global( closest, lcoords, FEIElementGeometryWrapper(element) );
        return distance(closest, gcoords);
    } else {
        closest = gcoords;
        return 0.0;
    }
}


int
SpatialLocalizerInterface :: SpatialLocalizerI_BBoxContainsPoint(const FloatArray &coords)
{
    FloatArray coordMin, coordMax;
    this->SpatialLocalizerI_giveBBox(coordMin, coordMax);

    int size = min( coordMin.giveSize(), coords.giveSize() );
    for ( int j = 1; j <= size; j++ ) {
        if ( coords.at(j) < coordMin.at(j) || coords.at(j) > coordMax.at(j) ) {
            return 0;
        }
    }

    return 1;
}



void
SpatialLocalizer :: giveAllElementsWithNodesWithinBox(elementContainerType &elemSet, const FloatArray &coords,
                                                      const double radius)
{
    nodeContainerType nodesWithinBox;
    const IntArray *dofmanConnectivity;

    elemSet.clear();

    ConnectivityTable *ct = domain->giveConnectivityTable();

    this->giveAllNodesWithinBox(nodesWithinBox, coords, radius);

    for ( int node: nodesWithinBox ) {
        dofmanConnectivity = ct->giveDofManConnectivityArray(node);
        for ( int i = 1; i <= dofmanConnectivity->giveSize(); i++ ) {
            elemSet.insertSortedOnce( dofmanConnectivity->at(i) );
        }
    }
}
} // end namespace oofem
