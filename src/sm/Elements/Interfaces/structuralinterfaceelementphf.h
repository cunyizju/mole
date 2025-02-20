// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef structuralinterfaceelementphf_h
#define structuralinterfaceelementphf_h

#include "sm/Elements/Interfaces/structuralinterfaceelement.h"
#include "sm/CrossSections/structuralinterfacecrosssection.h"
#include "input/element.h"
#include "math/floatmatrix.h"
#include "func/function.h"
#include "material/matresponsemode.h"
#include "input/valuemodetype.h"
#include "math/integrationdomain.h"
#include "dofman/dofmantransftype.h"

namespace oofem {
class TimeStep;
class Node;
class StructuralInterfaceMaterial;
class GaussPoint;
class FloatArray;
class IntArray;
class FEInterpolation;


/**
 * Interface element class with phase field (PhF) modeling of damage
 */
class StructuralInterfaceElementPhF : public StructuralInterfaceElement
{
protected:
    FEInterpolation *interpolation = nullptr;

    IntArray loc_u, loc_d;

public:
    /**
     * Constructor. Creates structural element with given number, belonging to given domain.
     * @param n Element number.
     * @param d Domain to which new material will belong.
     */
    StructuralInterfaceElementPhF(int n, Domain * d);

    //FEInterpolation *giveInterpolation() const override { return interpolation; };

    void computeStiffnessMatrix(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep) override;

    //Phf
    void computeStiffnessMatrix_uu(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep);
    void computeStiffnessMatrix_ud(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep);
    void computeStiffnessMatrix_dd(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep);
    void computeStiffnessMatrix_du(FloatMatrix &answer, MatResponseMode rMode, TimeStep *tStep);

    void giveInternalForcesVector_u(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord = 0);
    void giveInternalForcesVector_d(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord = 0);    

    virtual void giveDofManDofIDMask_u(IntArray &answer) = 0;
    virtual void giveDofManDofIDMask_d(IntArray &answer) = 0;
    virtual void computeLocationArrayOfDofIDs( const IntArray &dofIdArray, IntArray &answer );

    virtual void getLocationArray_u( IntArray &answer )=0;
    virtual void getLocationArray_d( IntArray &answer )=0;

    //remove
    int computeNumberOfDofs() override;

    ///@todo move these to a cross section model later
    double internalLength;
    double giveInternalLength( ) { return internalLength; };
    //double criticalEnergy;
    //double giveCriticalEnergy() { return criticalEnergy; };
    //double relaxationTime;
    //double giveRelaxationTime( ) { return relaxationTime; };
    //double penaltyParameter;
    //double givePenaltyParameter() { return penaltyParameter; };
    //double psiBar0;
    //double givePsiBar0() { return psiBar0; };

    ///@todo put in material model
    double computeFreeEnergy( GaussPoint *gp, TimeStep *tStep );

    virtual double computeDamageAt(GaussPoint *gp, ValueModeType valueMode, TimeStep *stepN);

    void computeDisplacementUnknowns(FloatArray &answer, ValueModeType valueMode, TimeStep *stepN);
    void computeDamageUnknowns(FloatArray &answer, ValueModeType valueMode, TimeStep *stepN);

    void computeBStress_u(FloatArray &answer, GaussPoint *gp, TimeStep *tStep, int useUpdatedGpRecord);
    void computeNStress_d( FloatArray &answer, GaussPoint *gp, TimeStep *tStep, int useUpdatedGpRecord );

    //Interpolation matrices for phase field
    virtual void computeBd_matrixAt(GaussPoint *, FloatMatrix &);
    virtual void computeNd_matrixAt(const FloatArray &lCoords, FloatMatrix &N);

    void giveInternalForcesVector(FloatArray &answer, TimeStep *tStep, int useUpdatedGpRecord = 0) override;
    virtual void computeTraction(FloatArray &traction, IntegrationPoint *ip, FloatArray &jump, TimeStep *tStep);
    //virtual void computeSpatialJump(FloatArray &answer, GaussPoint *gp, TimeStep *tStep);
    //int giveIPValue(FloatArray &answer, GaussPoint *gp, InternalStateType type, TimeStep *tStep) override;

    virtual void computeCovarBaseVectorsAt(GaussPoint *gp, FloatMatrix &G) = 0;
    //@}

    // Overloaded methods.
    void updateInternalState(TimeStep *tStep) override;
    void updateYourself(TimeStep *tStep) override;
    //int checkConsistency() override;
    //void initializeFrom(InputRecord &ir) override;
    //void giveInputRecord(DynamicInputRecord &input) override;
    const char *giveClassName() const override { return "StructuralInterfaceElementPhF"; };

    //StructuralInterfaceCrossSection *giveInterfaceCrossSection();

    //virtual methods that should be overloaded by the elements
    virtual void giveEngTraction(FloatArray &answer, GaussPoint *gp, const FloatArray &jump, double damage, TimeStep *tStep)
    {
        OOFEM_ERROR("not implemented for the current element");
    }
};
} // end namespace oofem
#endif // structuralinterfaceelement_h
