// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "Elasticity/linearelasticmaterial.h"
#include "anisolinearelasticmaterial.h"
#include "Structural/structuralms.h"
#include "math/floatmatrix.h"
#include "engng/classfactory.h"
#include "input/dynamicinputrecord.h"

namespace oofem {
REGISTER_Material(AnisotropicLinearElasticMaterial);

void
AnisotropicLinearElasticMaterial :: initializeFrom(InputRecord &ir)
{
    LinearElasticMaterial :: initializeFrom(ir);

    // read the stiffness coefficients arranged by rows from the diagonal to the right (21 values)
    FloatArray stiffness;
    IR_GIVE_FIELD(ir, stiffness, _IFT_AnisotropicLinearElasticMaterial_stiff);
    if ( stiffness.giveSize() != 21 ) {
        OOFEM_ERROR( "Incorrect size of stiff - should be 21, is %d\n", stiffness.giveSize() );
    }

    // put the stiffness coefficients into a 6x6 matrix
    for ( int k = 1, i = 1; i <= 6; i++ ) {
        tangent.at(i, i) = stiffness.at(k++);
        for ( int j = i + 1; j <= 6; j++ ) {
            tangent.at(i, j) = tangent.at(j, i) = stiffness.at(k++);
        }
    }
    this->computesSubTangents();

    FloatArray alpha_input(6);
    IR_GIVE_FIELD(ir, alpha_input, _IFT_AnisotropicLinearElasticMaterial_talpha);
    if ( alpha_input.giveSize() != 6 ) {
        OOFEM_ERROR( "Incorrect size of talpha - should be 6, is %d\n", alpha.giveSize() );
    }
    alpha = alpha_input;
}


void
AnisotropicLinearElasticMaterial :: giveInputRecord(DynamicInputRecord &input)
{
    Material :: giveInputRecord(input);
    FloatArray stiffness(21);
    for ( int k = 1, i = 1; i <= 6; i++ ) {
        for ( int j = i; j <= 6; j++ ) {
            stiffness.at(k++) = tangent.at(i, j);
        }
    }
    input.setField(stiffness, _IFT_AnisotropicLinearElasticMaterial_stiff);

    input.setField(alpha, _IFT_AnisotropicLinearElasticMaterial_talpha);
}

} // end namespace oofem
