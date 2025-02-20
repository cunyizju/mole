// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef structuralmaterialevaluator_h
#define structuralmaterialevaluator_h

#include "engng/engngm.h"

#include <fstream>

///@name Input fields for structural material evaluator
//@{
#define _IFT_StructuralMaterialEvaluator_Name "structuralmaterialevaluator"
#define _IFT_StructuralMaterialEvaluator_deltat "deltat"
#define _IFT_StructuralMaterialEvaluator_numberOfTimeSteps "nsteps"
#define _IFT_StructuralMaterialEvaluator_componentFunctions "componentfunctions" ///< Integer list of time functions for each component
#define _IFT_StructuralMaterialEvaluator_stressControl "stresscontrol" ///< Integer list of the stress components which are controlled
#define _IFT_StructuralMaterialEvaluator_outputVariables "vars" ///< Variables (from integration point) to be written.
#define _IFT_StructuralMaterialEvaluator_tolerance "tolerance" ///< Tolerance for stress control
#define _IFT_StructuralMaterialEvaluator_keepTangent "keeptangent"
//@}

namespace oofem {
class GaussPoint;

/**
 * For testing material behavior, particularly useful for multiscale modeling where one can test a single RVE.
 * The deviatoric and volumetric parts are split. No nodes or elements are used.
 *
 * This model will output data in its own way since it does not contain any actual FE-results so no export modules are called.
 * @author Mikael Öhman
 */
class StructuralMaterialEvaluator : public EngngModel
{
protected:
    double deltaT; ///< Time increment.
    double keepTangent;

    IntArray cmpntFunctions; /// Time functions controlling each component of the deviatoric part of the stress.
    IntArray sControl, eControl;
    IntArray vars;

    std::vector< std :: unique_ptr< GaussPoint > >gps;

    std :: ofstream outfile;

    double tolerance;

public:
    StructuralMaterialEvaluator(int i, EngngModel *master = nullptr);
    virtual ~StructuralMaterialEvaluator();

    void initializeFrom(InputRecord &ir) override;

    void solveYourself() override;

    int checkConsistency() override;
    void doStepOutput(TimeStep *tStep) override;
    TimeStep *giveNextStep() override;

    const char *giveClassName() const override { return "StructuralMaterialEvaluator"; }
    const char *giveInputRecordName() const { return _IFT_StructuralMaterialEvaluator_Name; }
};
} // end namespace oofem

#endif // structuralmaterialevaluator_h
