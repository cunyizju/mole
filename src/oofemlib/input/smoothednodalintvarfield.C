// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "input/smoothednodalintvarfield.h"
#include "mesher/spatiallocalizer.h"
#include "engng/classfactory.h"
#include "input/element.h"
#include "fei/feinterpol.h"
#include "dofman/dofmanager.h"
#include "utility/set.h"

namespace oofem {
SmoothedNodalInternalVariableField :: SmoothedNodalInternalVariableField(InternalStateType ist, FieldType ft, NodalRecoveryModel :: NodalRecoveryModelType st, Domain *d) : Field(ft)
{
    this->istType = ist;
    this->stype = st;
    this->smoother = classFactory.createNodalRecoveryModel(this->stype, d);
    this->domain = d;
}

SmoothedNodalInternalVariableField :: ~SmoothedNodalInternalVariableField() { }

int
SmoothedNodalInternalVariableField :: evaluateAt(FloatArray &answer, const FloatArray &coords, ValueModeType mode, TimeStep *tStep)
{
    int result = 0; // assume ok
    FloatArray lc, n;
    const FloatArray *nodalValue;

    // use whole domain recovery
    // create a new set containing all elements
    Set elemSet(0, this->domain);
    elemSet.addAllElements();
    this->smoother->recoverValues(elemSet, istType, tStep);
    // request element containing target point
    Element *elem = this->domain->giveSpatialLocalizer()->giveElementContainingPoint(coords);
    if ( elem ) { // ok element containing target point found
        FEInterpolation *interp = elem->giveInterpolation();
        if ( interp ) {
            // map target point to element local coordinates
            if ( interp->global2local( lc, coords, FEIElementGeometryWrapper(elem) ) ) {
                // evaluate interpolation functions at target point
                interp->evalN( n, lc, FEIElementGeometryWrapper(elem) );
                // loop over element nodes
                for ( int i = 1; i <= n.giveSize(); i++ ) {
                    // request nodal value
                    this->smoother->giveNodalVector( nodalValue, elem->giveDofManagerNumber(i) );
                    // multiply nodal value by value of corresponding shape function and add this to answer
                    answer.add(n.at(i), * nodalValue);
                }
            } else { // mapping from global to local coordinates failed
                result = 1; // failed
            }
        } else {  // element without interpolation
            result = 1; // failed
        }
    } else { // no element containing given point found
        result = 1; // failed
    }
    return result;
}

int
SmoothedNodalInternalVariableField :: evaluateAt(FloatArray &answer, DofManager *dman, ValueModeType mode, TimeStep *tStep)
{
    const FloatArray *val;
    int result = this->smoother->giveNodalVector( val, dman->giveNumber() );
    answer = * val;
    return ( result == 1 );
}

void
SmoothedNodalInternalVariableField :: saveContext(DataStream &stream)
{
}

void
SmoothedNodalInternalVariableField :: restoreContext(DataStream &stream)
{
}
} // end namespace oofem
