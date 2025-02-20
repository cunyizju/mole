// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef simpleinterfacemat_h
#define simpleinterfacemat_h

#include "sm/Materials/Interface/structuralinterfacematerial.h"
#include "sm/Materials/Interface/structuralinterfacematerialstatus.h"

///@name Input fields for SimpleInterfaceMaterial
//@{
#define _IFT_SimpleInterfaceMaterial_Name "simpleintermat"
#define _IFT_SimpleInterfaceMaterial_kn "kn"
#define _IFT_SimpleInterfaceMaterial_ks "ks"
#define _IFT_SimpleInterfaceMaterial_frictCoeff "fc"
#define _IFT_SimpleInterfaceMaterial_stiffCoeff "stiffcoeff"
#define _IFT_SimpleInterfaceMaterial_normalClearance "normalclearance"
#define _IFT_SimpleInterfaceMaterial_regularizedModel "regularized"
#define _IFT_SimpleInterfaceMaterial_regularizationCoeff "m"
//@}

namespace oofem {
/**
 * This class implements associated Material Status to SimpleInterfaceMaterial.
 */
class SimpleInterfaceMaterialStatus : public StructuralInterfaceMaterialStatus
{
protected:
    bool shearYieldingFlag = false;
    FloatArrayF<2> shearStressShift, tempShearStressShift;

public:
    /// Constructor
    SimpleInterfaceMaterialStatus(GaussPoint * g);

    void printOutputAt(FILE *file, TimeStep *tStep) const override;

    const char *giveClassName() const override { return "SimpleInterfaceMaterialStatus"; }

    void initTempStatus() override;
    void updateYourself(TimeStep *tStep) override;

    const FloatArrayF<2> &giveShearStressShift() const { return shearStressShift; }
    void setTempShearStressShift(const FloatArrayF<2> &newShearStressShift) { tempShearStressShift = newShearStressShift; }
    bool giveShearYieldingFlag() { return shearYieldingFlag; }
    void setShearYieldingFlag(bool sY) { shearYieldingFlag = sY; }

    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;
};


/**
 * Base class representing general isotropic damage model.
 * It is based on isotropic damage concept, assuming that damage evolution law
 * is postulated in explicit form, relatin damage parameter (omega) to scalar measure
 * of the largest strain level ever reached in material (kappa).
 */
class SimpleInterfaceMaterial : public StructuralInterfaceMaterial
{
protected:
    double kn = 0., ks = 0.;
    double stiffCoeff = 0.;
    double frictCoeff = 0.;
    /// Normal distance which needs to be closed when interface element should act in compression (distance is 0 by default).
    double normalClearance = 0.;

  bool regularizedModel = false;
  double m = 15.0;
  
public:
    /// Constructor
    SimpleInterfaceMaterial(int n, Domain * d);

    bool hasAnalyticalTangentStiffness() const override { return true; }

    const char *giveInputRecordName() const override { return _IFT_SimpleInterfaceMaterial_Name; }
    const char *giveClassName() const override { return "SimpleInterfaceMaterial"; }

    FloatArrayF<3> giveEngTraction_3d(const FloatArrayF<3> &jump, GaussPoint *gp, TimeStep *tStep) const override;
    FloatMatrixF<3,3> give3dStiffnessMatrix_Eng(MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) const override;

    int giveIPValue(FloatArray &answer, GaussPoint *gp, InternalStateType type, TimeStep *tStep) override;

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    MaterialStatus *CreateStatus(GaussPoint *gp) const override { return new SimpleInterfaceMaterialStatus(gp); }
};
} // end namespace oofem
#endif // simpleinterfacemat_h
