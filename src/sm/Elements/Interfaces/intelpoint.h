// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef intelpoint_h
#define intelpoint_h

#include "sm/Elements/Interfaces/structuralinterfaceelement.h"
#include "math/gaussintegrationrule.h"
#include "math/floatmatrixf.h"
//#include "math/floatarrayf.h"

///@name Input fields for Material
//@{
#define _IFT_IntElPoint_Name "intelpoint"
#define _IFT_IntElPoint_refnode "refnode"
#define _IFT_IntElPoint_normal "normal"
#define _IFT_IntElPoint_area "area"
#define _IFT_IntElPoint_length "length"
//@}

namespace oofem {
/**
 * This class implements an interface element that connects two nodes.
 * In order to compute the normal and tangential direction of the slip plane, 
 * a reference node or specific direction can be specified in the input.
 * If neither are specified, then the normal direction are computed from the coordinates
 * of the element's nodes.
 * The class adjusts dimensionality automatically to 1D, 2D, or 3D depending on domain.
 * @author Jim Brouzoulis
 * @author Borek Patzak
 */
class IntElPoint : public StructuralInterfaceElement
{
protected:
    enum cmode { ie1d_1d, ie1d_2d, ie1d_3d } mode;
    int referenceNode = 0;
    FloatArrayF<3> normal;
    double area = 0.;
    double length = 0.;

public:
    IntElPoint(int n, Domain *d);

    int computeGlobalCoordinates(FloatArray &answer, const FloatArray &lcoords) override;

    int computeNumberOfDofs() override;
    void giveDofManDofIDMask(int inode, IntArray &answer) const override;

    double computeAreaAround(GaussPoint *gp) override;

    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_IntElPoint_Name; }
    void initializeFrom(InputRecord &ir) override;
    Element_Geometry_Type giveGeometryType() const override { return EGT_line_1; }

    MaterialMode giveMaterialMode() override;

    void computeTransformationMatrixAt(GaussPoint *gp, FloatMatrix &answer) override;

    void giveEngTraction(FloatArray &answer, GaussPoint *gp, const FloatArray &jump, TimeStep *tStep) override
    {
        if ( this->giveDomain()->giveNumberOfSpatialDimensions() == 3 ) {
            answer = this->giveInterfaceCrossSection()->giveEngTraction_3d(jump, gp, tStep);
        } else if ( this->giveDomain()->giveNumberOfSpatialDimensions() == 2 ) {
            answer = this->giveInterfaceCrossSection()->giveEngTraction_2d(jump, gp, tStep);
        } else if ( this->giveDomain()->giveNumberOfSpatialDimensions() == 1 ) {
            answer = {this->giveInterfaceCrossSection()->giveEngTraction_1d(jump.at(1), gp, tStep)};
        }
    }

    void giveStiffnessMatrix_Eng(FloatMatrix &answer, MatResponseMode rMode, IntegrationPoint *ip, TimeStep *tStep) override
    {
        if ( this->giveDomain()->giveNumberOfSpatialDimensions() == 3 ) {
            answer = this->giveInterfaceCrossSection()->give3dStiffnessMatrix_Eng(rMode, ip, tStep);
        } else if ( this->giveDomain()->giveNumberOfSpatialDimensions() == 2 ) {
            answer = this->giveInterfaceCrossSection()->give2dStiffnessMatrix_Eng(rMode, ip, tStep);
        } else if ( this->giveDomain()->giveNumberOfSpatialDimensions() == 1 ) {
            answer = this->giveInterfaceCrossSection()->give1dStiffnessMatrix_Eng(rMode, ip, tStep);
        }
    }



protected:
    void computeNmatrixAt(GaussPoint *gp, FloatMatrix &answer) override;
    void computeGaussPoints() override;

    //    void computeLocalSlipDir(FloatArrayF &normal);
    //    void computeLocalSlipDir(FloatArrayF<3> normal);
        void computeLocalSlipDir(void);    
    cmode giveCoordMode() const { return this->mode; }
    void setCoordMode();
};
} // end namespace oofem
#endif // interfaceelement1d_h
