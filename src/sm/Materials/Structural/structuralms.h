// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef structuralms_h
#define structuralms_h

#include "material/matstatus.h"
#include "math/floatarray.h"
#include "mapping/matstatmapperint.h"

namespace oofem {
class GaussPoint;
class Dictionary;
class Domain;

/**
 * This class implements a structural material status information. It is attribute of
 * gaussPoint. This is only an abstract class, for every instance of material class
 * there should be specialized derived class, which handles are history variables.
 *
 * This is a base class for all material statuses corresponding to materials derived from
 * structural material class.
 * It defines stress and strain vectors and their increments.
 * Functions for accessing these components are defined.
 *
 * Tasks:
 * This is abstract class - only basic functionality is supported like:
 * - maintaining and providing access to stress and strain vectors
 *   (including their increments)
 * - storing and restoring status on tape
 * - printingYourself()
 * - updating Yourself after a new equilibrium state has been reached.
 */
class OOFEM_EXPORT StructuralMaterialStatus : public MaterialStatus, public MaterialStatusMapperInterface
{
protected:
    /// Equilibrated strain vector in reduced form
    FloatArray strainVector;
    /// Equilibrated stress vector in reduced form
    FloatArray stressVector;
    /// Temporary stress vector in reduced form (increments are used mainly in nonlinear analysis)
    FloatArray tempStressVector;
    /// Temporary strain vector in reduced form (to find balanced state)
    FloatArray tempStrainVector;

    /// Equilibrated first Piola-Kirchhoff stress vector
    FloatArray PVector;
    /// Temporary first Piola-Kirchhoff stress vector (to find balanced state)
    FloatArray tempPVector;
    /// Equilibrated Cauchy stress vector
    FloatArray CVector;
    /// Temporary Cauchy stress vector (to find balanced state)
    FloatArray tempCVector;
    /// Equilibrated deformation gradient in reduced form
    FloatArray FVector;
    /// Temporary deformation gradient in reduced form (to find balanced state)
    FloatArray tempFVector;

public:
    /// Constructor. Creates new StructuralMaterialStatus with IntegrationPoint g.
    StructuralMaterialStatus(GaussPoint * g);

    void printOutputAt(FILE *file, TimeStep *tStep) const override;

    void initTempStatus() override;
    void updateYourself(TimeStep *tStep) override;

    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;

    /// Returns the const pointer to receiver's strain vector.
    const FloatArray &giveStrainVector() const { return strainVector; }
    /// Returns the const pointer to receiver's stress vector.
    const FloatArray &giveStressVector() const { return stressVector; }
    /// Returns the const pointer to receiver's first Piola-Kirchhoff stress vector.
    const FloatArray &givePVector() const { return PVector; }
    /// Returns the const pointer to receiver's Cauchy stress vector.
    const FloatArray &giveCVector() const { return CVector; }
    /// Returns the const pointer to receiver's deformation gradient vector.
    const FloatArray &giveFVector() const { return FVector; }
    /// Returns the const pointer to receiver's temporary strain vector.
    const FloatArray &giveTempStrainVector() const { return tempStrainVector; }
    /// Returns the const pointer to receiver's temporary stress vector.
    const FloatArray &giveTempStressVector() const { return tempStressVector; }
    /// Returns the const pointer to receiver's temporary first Piola-Kirchhoff stress vector.
    const FloatArray &giveTempPVector() const { return tempPVector; }
    /// Returns the const pointer to receiver's temporary Cauchy stress vector.
    const FloatArray &giveTempCVector() const { return tempCVector; }
    /// Returns the const pointer to receiver's temporary deformation gradient vector.
    const FloatArray &giveTempFVector() const { return tempFVector; }
    /// Assigns strain vector to given vector v.
    void letStrainVectorBe(const FloatArray &v) { strainVector = v; }
    /// Assigns stressVector to given vector v.
    void letStressVectorBe(const FloatArray &v) { stressVector = v; }
    /// Assigns PVector to given vector v.
    void letPVectorBe(const FloatArray &v) { PVector = v; }
    /// Assigns CVector to given vector v.
    void letCVectorBe(const FloatArray &v) { CVector = v; }
    /// Assigns FVector to given vector v.
    void letFVectorBe(const FloatArray &v) { FVector = v; }
    /// Assigns tempStressVector to given vector v.
    void letTempStressVectorBe(const FloatArray &v) { tempStressVector = v; }
    /// Assigns tempStrainVector to given vector v
    void letTempStrainVectorBe(const FloatArray &v) { tempStrainVector = v; }
    /// Assigns tempPVector to given vector v
    void letTempPVectorBe(const FloatArray &v) { tempPVector = v; }
    /// Assigns tempPVector to given vector v
    void letTempCVectorBe(const FloatArray &v) { tempCVector = v; }
    /// Assigns tempFVector to given vector v
    void letTempFVectorBe(const FloatArray &v) { tempFVector = v; }

    const char *giveClassName() const override { return "StructuralMaterialStatus"; }

    /// Functions for MaterialStatusMapperInterface
    void copyStateVariables(const MaterialStatus &iStatus) override;
    void addStateVariables(const MaterialStatus &iStatus) override;
};
} // end namespace oofem
#endif // structuralms_h
