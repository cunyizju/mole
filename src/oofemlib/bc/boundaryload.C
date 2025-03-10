// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "bc/boundaryload.h"
#include "func/function.h"
#include "math/floatarray.h"
#include "solvers/timestep.h"
#include "input/dynamicinputrecord.h"
#include "input/valuemodetype.h"
#include "input/domain.h"
#include "export/datastream.h"
#include "utility/contextioerr.h"

namespace oofem {

BoundaryLoad :: BoundaryLoad(int i, Domain * d) : Load(i, d), coordSystemType(CST_Global){

}

void
BoundaryLoad :: computeComponentArrayAt(FloatArray &answer, TimeStep *tStep, ValueModeType mode)
{
    // returns component array for elements which use direct formulae
    Load :: computeComponentArrayAt(answer, tStep, mode);
}


void
BoundaryLoad :: computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode)
{
    // Evaluates the value at specific integration point
    int nSize, nDofs;
    double factor;
    FloatArray N;

    if ( ( mode != VM_Total ) && ( mode != VM_Incremental ) ) {
        OOFEM_ERROR("unknown mode");
    }

    this->computeNArray(N, coords);
    nSize = N.giveSize();

    nDofs = this->componentArray.giveSize() / nSize;

    answer.resize(nDofs);

    for ( int i = 1; i <= nDofs; i++ ) {
        double value = 0.;
        for ( int j = 1; j <= nSize; j++ ) {
            value += N.at(j) * this->componentArray.at(i + ( j - 1 ) * nDofs);
        }

        answer.at(i) = value;
    }

    // time distribution
    factor = this->giveTimeFunction()->evaluate(tStep, mode);
    answer.times(factor);
}


void
BoundaryLoad :: initializeFrom(InputRecord &ir)
{
    Load :: initializeFrom(ir);

    int dummy;
    IR_GIVE_OPTIONAL_FIELD(ir, dummy, "ndofs");

    int value = 0;
    IR_GIVE_OPTIONAL_FIELD(ir, value, _IFT_BoundaryLoad_loadtype);
    lType = ( bcType ) value;

    value = 0;
    IR_GIVE_OPTIONAL_FIELD(ir, value, _IFT_BoundaryLoad_cstype);
    coordSystemType = ( CoordSystType ) value;

    IR_GIVE_OPTIONAL_FIELD(ir, propertyDictionary, _IFT_BoundaryLoad_properties);
    IR_GIVE_OPTIONAL_FIELD(ir, propertyTimeFunctDictionary, _IFT_BoundaryLoad_propertyTimeFunctions);

    IR_GIVE_OPTIONAL_FIELD(ir, propertyMultExpr, _IFT_BoundaryLoad_propertyMultExpr);

    temperOffset = 273.15;
    IR_GIVE_OPTIONAL_FIELD(ir, temperOffset, _IFT_BoundaryLoad_temperOffset);
}


void
BoundaryLoad :: giveInputRecord(DynamicInputRecord &input)
{
    Load :: giveInputRecord(input);
    input.setField(this->lType, _IFT_BoundaryLoad_loadtype);
    input.setField(this->coordSystemType, _IFT_BoundaryLoad_cstype);
    input.setField(this->propertyDictionary, _IFT_BoundaryLoad_properties);
    input.setField(this->propertyTimeFunctDictionary, _IFT_BoundaryLoad_propertyTimeFunctions);
    input.setField(this->propertyMultExpr, _IFT_BoundaryLoad_propertyMultExpr);
}


double
BoundaryLoad :: giveProperty(int aProperty, TimeStep *tStep, const std :: map< std :: string, FunctionArgument > &valDict)
{
    double answer;
    if ( propertyDictionary.includes(aProperty) ) {
        // check if time fuction registered under the same key
        if ( propertyTimeFunctDictionary.includes(aProperty) ) {
            answer = propertyDictionary.at(aProperty) * domain->giveFunction( (int)propertyTimeFunctDictionary.at(aProperty) )->evaluate(tStep, VM_Total);
        } else {
            answer = propertyDictionary.at(aProperty);
        }
    } else {
        OOFEM_ERROR("Property '%c' not defined", (char)aProperty);
    }

    if ( propertyMultExpr.isDefined() ) {
        answer *= propertyMultExpr.eval( valDict, this->giveDomain() );
    }
    return answer;
}

double
BoundaryLoad :: giveProperty(int aProperty, TimeStep *tStep)
{
    return this->giveProperty(aProperty, tStep, {});
}

double
BoundaryLoad :: giveTemperOffset(void)
{
    return this->temperOffset;
}

void
BoundaryLoad :: saveContext(DataStream &stream, ContextMode mode)
{
    Load :: saveContext(stream, mode);

    if ( mode & CM_Definition ) {
        if ( !stream.write(lType) ) {
          THROW_CIOERR(CIO_IOERR);
        }
        if ( !stream.write(coordSystemType) ) {
          THROW_CIOERR(CIO_IOERR);
        }
        propertyDictionary.saveContext(stream);
        propertyTimeFunctDictionary.saveContext(stream);

        if ( !stream.write(temperOffset) ) {
          THROW_CIOERR(CIO_IOERR);
        }
    }
}


void
BoundaryLoad :: restoreContext(DataStream &stream, ContextMode mode)
{
    Load :: restoreContext(stream, mode);

    if ( mode & CM_Definition ) {
        int _val;
        if ( !stream.read(_val) ) {
          THROW_CIOERR(CIO_IOERR);
        }
        lType = (bcType) _val;

        if ( !stream.read(_val) ) {
          THROW_CIOERR(CIO_IOERR);
        }
        coordSystemType = (CoordSystType) _val;

        propertyDictionary.restoreContext(stream);
        propertyTimeFunctDictionary.restoreContext(stream);

        if ( !stream.read(temperOffset) ) {
          THROW_CIOERR(CIO_IOERR);
        }
    }
}

} // end namespace oofem
