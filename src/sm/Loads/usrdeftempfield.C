// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "usrdeftempfield.h"
#include "solvers/timestep.h"
#include "engng/classfactory.h"

#include <sstream>

namespace oofem {
REGISTER_BoundaryCondition(UserDefinedTemperatureField);

void
UserDefinedTemperatureField :: computeValueAt(FloatArray &answer, TimeStep *tStep, const FloatArray &coords, ValueModeType mode)
// Returns the value of the receiver at time and given position respecting the mode.
{
    int err;
    double result;

    if ( ( mode != VM_Incremental ) && ( mode != VM_Total ) ) {
        OOFEM_ERROR("unknown mode (%s)", __ValueModeTypeToString(mode) );
    }

    answer.resize(this->size);
    std :: ostringstream buff;
    for ( int i = 1; i <= size; i++ ) {
        buff << "x=" << coords.at(1) << ";y=" << coords.at(2) << ";z=" << coords.at(3) <<
        ";t=" << tStep->giveTargetTime() << ";" << ftExpression [ i - 1 ];
        result = myParser.eval(buff.str().c_str(), err);
        if ( err ) {
            OOFEM_ERROR("parser syntax error");
        }

        answer.at(i) = result;

        if ( ( mode == VM_Incremental ) && ( !tStep->isTheFirstStep() ) ) {
            buff << "x=" << coords.at(1) << ";y=" << coords.at(2) << ";z=" << coords.at(3) <<
            ";t=" << ( tStep->giveTargetTime() - tStep->giveTimeIncrement() ) << ";" << ftExpression [ i - 1 ];
            result = myParser.eval(buff.str().c_str(), err);
            if ( err ) {
                OOFEM_ERROR("parser syntax error");
            }

            answer.at(i) -= result;
        }
    }
}

void
UserDefinedTemperatureField :: initializeFrom(InputRecord &ir)
{
    IR_GIVE_FIELD(ir, size, _IFT_UserDefinedTemperatureField_size);
    if ( size > 3 ) {
        size = 3;
    }

    if ( size > 0 ) {
        IR_GIVE_FIELD(ir, ftExpression [ 0 ], _IFT_UserDefinedTemperatureField_t1);
    }

    if ( size > 1 ) {
        IR_GIVE_FIELD(ir, ftExpression [ 1 ], _IFT_UserDefinedTemperatureField_t2);
    }

    if ( size > 2 ) {
        IR_GIVE_FIELD(ir, ftExpression [ 2 ], _IFT_UserDefinedTemperatureField_t3);
    }
}
} // end namespace oofem
