// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "rotatingboundary.h"
#include "dofman/dofmanager.h"
#include "dofman/dof.h"
#include "math/mathfem.h"
#include "func/function.h"
#include "engng/classfactory.h"
#include "input/dynamicinputrecord.h"

namespace oofem {
REGISTER_BoundaryCondition(RotatingBoundary);

double RotatingBoundary :: give(Dof *dof, ValueModeType mode, double time)
{
    DofIDItem id = dof->giveDofID();
    const auto &coords = dof->giveDofManager()->giveCoordinates();
    FloatArray answer, newcoords;
    double theta = 0.;

    if ( mode == VM_Total ) {
        theta = this->giveTimeFunction()->evaluateAtTime(time);
    } else if ( mode == VM_Velocity ) {
        theta = this->giveTimeFunction()->evaluateVelocityAtTime(time);
    } else if ( mode == VM_Acceleration ) {
        theta = this->giveTimeFunction()->evaluateAccelerationAtTime(time);
    } else {
        OOFEM_ERROR("Should not be called for value mode type then total, velocity, or acceleration.");
    }

    if ( axis.giveSize() != 3 ) {
        OOFEM_ERROR("Size of rotation axis != 3.");
    }

    if ( center.giveSize() == 0 ) {
        center.resize( coords.giveSize() );
        center.zero();
    }

    if ( coords.giveSize() != center.giveSize() ) {
        OOFEM_ERROR("Size of coordinate system different from center of rotation.");
    }

    double &nx = axis.at(1);
    double &ny = axis.at(2);
    double &nz = axis.at(3);

    if ( coords.giveSize() == 1 ) {
        R.resize(1, 1);
        R.at(1, 1) = cos(theta) + nx * nx * ( 1 - cos(theta) );
    }
    if ( coords.giveSize() == 2 ) {
        R.resize(2, 2);
        R.at(1, 1) = cos(theta) + nx * nx * ( 1 - cos(theta) );
        R.at(1, 2) = nx * ny * ( 1 - cos(theta) ) - nz *sin(theta);
        R.at(2, 1) = ny * nx * ( 1 - cos(theta) ) + nz *sin(theta);
        R.at(2, 2) = cos(theta) + ny * ny * ( 1 - cos(theta) );
    } else if ( coords.giveSize() == 3  ) {
        R.resize(3, 3);

        R.at(1, 1) = cos(theta) + nx * nx * ( 1 - cos(theta) );
        R.at(1, 2) = nx * ny * ( 1 - cos(theta) ) - nz *sin(theta);
        R.at(1, 3) = nx * nz * ( 1 - cos(theta) ) + ny *sin(theta);

        R.at(2, 1) = ny * nx * ( 1 - cos(theta) ) + nz *sin(theta);
        R.at(2, 2) = cos(theta) + ny * ny * ( 1 - cos(theta) );
        R.at(2, 3) = ny * nz * ( 1 - cos(theta) ) - nx *sin(theta);

        R.at(3, 1) = nz * nx * ( 1 - cos(theta) ) - ny *sin(theta);
        R.at(3, 2) = nz * ny * ( 1 - cos(theta) ) + nx *sin(theta);
        R.at(3, 3) = cos(theta) + nz * nz * ( 1 - cos(theta) );
    } else {
        OOFEM_ERROR("Size of coordinate system has to be 1, 2 or 3.");
    }

    newcoords.beDifferenceOf(center, coords);
    answer.beProductOf(R, newcoords);
    answer.add(center);
    answer.subtract(coords);

    switch ( id ) {
    case D_u:
        return answer.at(1);

    case D_v:
        return answer.at(2);

    case D_w:
        return answer.at(3);

    default:
        return 0.0;
    }
}

void
RotatingBoundary :: initializeFrom(InputRecord &ir)
{
    GeneralBoundaryCondition :: initializeFrom(ir);

    IR_GIVE_FIELD(ir, axis, _IFT_RotatingBoundary_axis);
    axis.normalize();

    IR_GIVE_OPTIONAL_FIELD(ir, center, _IFT_RotatingBoundary_center);
}

void
RotatingBoundary :: giveInputRecord(DynamicInputRecord &input)
{
    GeneralBoundaryCondition :: giveInputRecord(input);
    input.setField(this->axis, _IFT_RotatingBoundary_axis);
    input.setField(this->center, _IFT_RotatingBoundary_center);
}
} // end namespace oofem
