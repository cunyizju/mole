// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef mfrontusermaterial_h
#define mfrontusermaterial_h

#include <memory>

#include "sm/Materials/Structural/structuralmaterial.h"
#include "sm/Materials/Structural/structuralms.h"
#include "math/floatarray.h"
#include "math/floatarrayf.h"
#include "math/floatmatrix.h"

#include "MGIS/Behaviour/State.hxx"
#include "MGIS/Behaviour/Behaviour.hxx"
#include "MGIS/Behaviour/BehaviourData.hxx"
#include "MGIS/Behaviour/Integrate.hxx"
#include "MGIS/ThreadPool.hxx"
#include "MGIS/Behaviour/MaterialDataManager.hxx"

///@name Input fields for MFrontUserMaterial
//@{
#define _IFT_MFrontUserMaterial_Name "mfrontusermaterial"
#define _IFT_MFrontUserMaterial_numState "numstate"
#define _IFT_MFrontUserMaterial_properties "properties"
#define _IFT_MFrontUserMaterial_initialStress "initialstress"
#define _IFT_MFrontUserMaterial_userMaterial "umat"
#define _IFT_MFrontUserMaterial_name "name"
#define _IFT_MFrontUserMaterial_numericalTangent "numericaltangent"
#define _IFT_MFrontUserMaterial_numericalTangentPerturbation "perturbation"
#define _IFT_MFrontUserMaterial_libpath "libpath"
#define _IFT_MFrontUserMaterial_modelname "modelname"
//@}


namespace oofem {


using namespace mgis;
using namespace mgis::behaviour;


class MFrontUserMaterial : public StructuralMaterial
{
private:

    char libname[200];
    char modelname[200];

    /// Material properties.
    FloatArray properties;
    /// Initial stress.
    FloatArrayF<6> initialStress;

    int mStressInterpretation = 0;

    bool mUseNumericalTangent = false;

    /**
     * Size of perturbation for numerical tangent.
     */
    double mPerturbation = 1e-7;

    std :: string filename;
    
    static int const mfront2oo9[9];
    static int const mfront2oo6[6];

    //! \brief pointer to the MGIS behaviour
    std::unique_ptr<mgis::behaviour::Behaviour> behaviour;

public:
    /// Constructor.
    MFrontUserMaterial(int n, Domain *d);
    /// Destructor.
    virtual ~MFrontUserMaterial();

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    MaterialStatus *CreateStatus(GaussPoint *gp) const override;

    FloatMatrixF<6,6> give3dMaterialStiffnessMatrix(MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) const override;

    FloatMatrixF<9,9> give3dMaterialStiffnessMatrix_dPdF(MatResponseMode mode, GaussPoint *gp, TimeStep *tStep) const override;

    FloatMatrixF<5,5> givePlaneStrainStiffMtrx_dPdF(MatResponseMode mmode, GaussPoint *gp, TimeStep *tStep) const override;

    FloatArrayF<6> giveRealStressVector_3d(const FloatArrayF<6> &strain, GaussPoint *gp, TimeStep *tStep) const override;

    int giveIPValue(FloatArray &answer, GaussPoint *gp, InternalStateType type, TimeStep *tStep) override;

    const char *giveClassName() const override { return "MFrontUserMaterial"; }
    const char *giveInputRecordName() const override { return _IFT_MFrontUserMaterial_Name; }
};

class MFrontUserMaterialStatus : public StructuralMaterialStatus
{
protected:
    /// General state vector.
    FloatArray stateVector;
    /// Temporary state vector.
    FloatArray tempStateVector;
    /// Temporary elastic tangent.
    FloatMatrix tempTangent;

    /// Checker to see if tangent has been computed.
    bool hasTangentFlag;

public:
    /// Constructor.
    MFrontUserMaterialStatus(GaussPoint *gp, const mgis::behaviour::Behaviour &b);

    void initTempStatus() override;
    void updateYourself(TimeStep *tStep) override;

    bool hasTangent() const { return hasTangentFlag; }

    const FloatArray &giveStateVector() const { return stateVector; }
    FloatArray &letStateVectorBe(FloatArray &s) { return stateVector = s; }
    const FloatArray &giveTempStateVector() const { return tempStateVector; }
    FloatArray &letTempStateVectorBe(FloatArray &s) { return tempStateVector = s; }
    const FloatMatrix &giveTempTangent() { return tempTangent; }
    void letTempTangentBe(FloatMatrix t) {
        tempTangent = std::move(t);
        hasTangentFlag = true;
    }

    void printOutputAt(FILE *file, TimeStep *tStep) const override;

    const char *giveClassName() const override { return "MFrontUserMaterialStatus"; }
};
} // end namespace oofem
#endif // mfrontusermaterial_h
