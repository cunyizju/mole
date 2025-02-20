// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef structuralpythonmaterial_h
#define structuralpythonmaterial_h

#include<boost/python.hpp>
namespace bp=boost::python;

#include "sm/Materials/Structural/structuralmaterial.h"
#include "sm/Materials/Structural/structuralms.h"


///@name Input fields for StructuralPythonMaterial
//@{
#define _IFT_StructuralPythonMaterial_Name "structuralpythonmaterial"
#define _IFT_StructuralPythonMaterial_moduleName "module" /// The name of the module with the supplied functions (i.e. the name of the python script, without file extension)
//@}

namespace oofem {
/**
 * Custom user supplied python scripts for material models.
 * The python module should contain the functions
 * @code{.py}
 * computeStress(oldStrain, oldStress, strain, state, time) # returns stress
 * computePK1Stress(oldF, oldP, F, state, time) # returns P
 * @endcode
 * and optionally
 * @code{.py}
 * computeStressTangent(strain, stress, state, time) # returns ds/de
 * computePK1StressTangent(F, P, state, time) # return dP/dF
 * @endcode
 * else numerical derivatives are used. The state variable should be a dictionary storing either doubles or arrays of doubles.
 * 
 * This code is still experimental, and needs extensive testing.
 * @author Mikael Öhman
 */
class StructuralPythonMaterial : public StructuralMaterial
{
private:
    /// Name of the file that contains the python function
    std :: string moduleName;
    /// module containing functions (created from moduleName)
    bp::object module;
    /// callable for small deformations
    bp::object smallDef, smallDefTangent;
    // callables for large deformations
    bp::object largeDef, largeDefTangent;

    /// Numerical pertubation for numerical tangents
    double pert = 1e-12;
public:
    /// Constructor.
    StructuralPythonMaterial(int n, Domain * d);

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    MaterialStatus *CreateStatus(GaussPoint *gp) const override;

    FloatArray callStressFunction(bp::object func, const FloatArray &oldStrain, const FloatArray &oldStress, const FloatArray &strain, bp::object stateDict, bp::object tempStateDict, TimeStep *tStep) const;
    
    FloatMatrix callTangentFunction(bp::object func, const FloatArray &oldStrain, const FloatArray &oldStress, bp::object stateDict, bp::object tempStateDict, TimeStep *tStep) const;
    
    

    void give3dMaterialStiffnessMatrix(FloatMatrix &answer,
                                       MatResponseMode mode, GaussPoint *gp, TimeStep *tStep); //TODO check override

    FloatMatrixF<9,9> give3dMaterialStiffnessMatrix_dPdF(MatResponseMode mode, GaussPoint *gp,
                                                         TimeStep *tStep) const override;

    void giveRealStressVector_3d(FloatArray &answer, GaussPoint *gp,
                                 const FloatArray &reducedStrain, TimeStep *tStep); //TODO check override

    FloatArrayF<9> giveFirstPKStressVector_3d(const FloatArrayF<9> &vF, GaussPoint *gp,
                                              TimeStep *tStep) const override;

    int giveIPValue(FloatArray &answer, GaussPoint *gp, InternalStateType type, TimeStep *tStep) override;

    const char *giveClassName() const override { return "StructuralPythonMaterial"; }
    const char *giveInputRecordName() const override { return _IFT_StructuralPythonMaterial_Name; }
};

class StructuralPythonMaterialStatus : public StructuralMaterialStatus
{
protected:
    /// Internal state variables
    bp::dict stateDict, tempStateDict;

public:
    /// Constructor.
    StructuralPythonMaterialStatus(GaussPoint * gp);

    void initTempStatus() override;
    void updateYourself(TimeStep *tStep) override;
    void reinitTempStateDictionary();

    bp::object giveStateDictionary() { return stateDict; }
    bp::object giveTempStateDictionary() { return tempStateDict; }

    const char *giveClassName() const override { return "StructuralPythonMaterialStatus"; }
};
} // end namespace oofem
#endif // structuralpythonmaterial_h
