// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "bc/prescribedgradienthomogenization.h"
#include "input/domain.h"
#include "input/dynamicinputrecord.h"
#include "utility/set.h"
#include "fei/feinterpol.h"
#include "input/element.h"
#include "math/mathfem.h"

namespace oofem {

void PrescribedGradientHomogenization :: initializeFrom(InputRecord &ir)
{
    IR_GIVE_FIELD(ir, mGradient, _IFT_PrescribedGradientHomogenization_gradient);

    mCenterCoord.resize( mGradient.giveNumberOfColumns() );
    mCenterCoord.zero();
    IR_GIVE_OPTIONAL_FIELD(ir, mCenterCoord, _IFT_PrescribedGradientHomogenization_centercoords)
}

void PrescribedGradientHomogenization :: giveInputRecord(DynamicInputRecord &input)
{
    input.setField(mGradient, _IFT_PrescribedGradientHomogenization_gradient);
    input.setField(mCenterCoord, _IFT_PrescribedGradientHomogenization_centercoords);
}


void PrescribedGradientHomogenization :: setPrescribedGradientVoigt(const FloatArray &t)
{
    int n = t.giveSize();
    if ( n == 3 ) { // Then 2D
        this->mGradient.resize(2, 2);
        this->mGradient.at(1, 1) = t.at(1);
        this->mGradient.at(2, 2) = t.at(2);
        // In voigt form, assuming the use of gamma_12 instead of eps_12
        this->mGradient.at(1, 2) = this->mGradient.at(2, 1) = t.at(3) * 0.5;
    } else if ( n == 4 ) { // Then 2D
        this->mGradient.resize(2, 2);
        this->mGradient.at(1, 1) = t.at(1);
        this->mGradient.at(2, 2) = t.at(2);
        // In voigt form, assuming the use of gamma_12 instead of eps_12
        this->mGradient.at(1, 2) = this->mGradient.at(2, 1) = t.at(4) * 0.5;
    } else if ( n == 6 ) { // Then 3D
        this->mGradient.resize(3, 3);
        this->mGradient.at(1, 1) = t.at(1);
        this->mGradient.at(2, 2) = t.at(2);
        this->mGradient.at(3, 3) = t.at(3);
        // In voigt form, assuming the use of gamma_12 instead of eps_12
        this->mGradient.at(1, 2) = this->mGradient.at(2, 1) = t.at(6) * 0.5;
        this->mGradient.at(1, 3) = this->mGradient.at(3, 1) = t.at(5) * 0.5;
        this->mGradient.at(2, 3) = this->mGradient.at(3, 2) = t.at(4) * 0.5;
    } else if ( n == 1 ) {
        this->mGradient.resize(1, 1);
        this->mGradient.at(1, 1) = t.at(1);
    } else {
        OOFEM_ERROR("Tensor is in strange voigt format. Should be 3 or 6. Use setPrescribedGradient directly if needed.");
    }
}


void PrescribedGradientHomogenization :: giveGradientVoigt(FloatArray &oGradient) const
{
    int numRows = mGradient.giveNumberOfRows();
    switch ( numRows ) {
    case 1:
        oGradient = FloatArray {
            mGradient.at(1, 1)
        };
        break;
    case 2:
        // Do not assume symmetry
        oGradient = {
            mGradient.at(1, 1), mGradient.at(2, 2), mGradient.at(1, 2), mGradient.at(2, 1)
        };
        break;
    case 3:
        // TODO: Fix this properly.
        oGradient = {
            mGradient.at(1, 1), mGradient.at(2, 2), mGradient.at(1, 2), mGradient.at(2, 1)
        };
//        OOFEM_ERROR("PrescribedGradientHomogenization :: giveGradientVoigt() not implemented for 3 rows.\n")
        break;
    }
}


double PrescribedGradientHomogenization :: domainSize(Domain *d, int setNum)
{
    int nsd = d->giveNumberOfSpatialDimensions();
    double domain_size = 0.0;
    // This requires the boundary to be consistent and ordered correctly.
    Set *set = d->giveSet(setNum);
    const IntArray &boundaries = set->giveBoundaryList();

    for ( int pos = 1; pos <= boundaries.giveSize() / 2; ++pos ) {
        Element *e = d->giveElement( boundaries.at(pos * 2 - 1) );
        int boundary = boundaries.at(pos * 2);
        FEInterpolation *fei = e->giveInterpolation();
        domain_size += fei->evalNXIntegral( boundary, FEIElementGeometryWrapper(e) );
    }
    return fabs(domain_size / nsd);
}
} /* namespace oofem */
