// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef igaelements_h
#define igaelements_h

#include "iga/iga.h"
#include "iga/feibspline.h"
#include "iga/feinurbs.h"
#include "iga/feitspline.h"
#include "sm/Elements/PlaneStress/planestresselementevaluator.h"
#include "sm/Elements/3D/space3delementevaluator.h"
#include "math/floatarray.h"
#include "math/floatmatrix.h"
#include "material/matresponsemode.h"
#include "math/mathfem.h"

#define _IFT_BsplinePlaneStressElement_Name "bsplineplanestresselement"
#define _IFT_NURBSPlaneStressElement_Name "nurbsplanestresselement"
#define _IFT_TSplinePlaneStressElement_Name "tsplineplanestresselement"
#define _IFT_NURBSSpace3dElement_Name "nurbs3delement"

namespace oofem {
class BsplinePlaneStressElement : public IGAElement, public PlaneStressStructuralElementEvaluator
{
protected:
    BSplineInterpolation interpolation;

public:
    BsplinePlaneStressElement(int n, Domain * aDomain);

    void initializeFrom(InputRecord &ir) override;
    int checkConsistency() override;

    void giveCharacteristicMatrix(FloatMatrix &answer, CharType mtrx, TimeStep *tStep) override {
        PlaneStressStructuralElementEvaluator :: giveCharacteristicMatrix(answer, mtrx, tStep);
    }
    void giveCharacteristicVector(FloatArray &answer, CharType type, ValueModeType mode, TimeStep *t) override {
        PlaneStressStructuralElementEvaluator :: giveCharacteristicVector(answer, type, mode, t);
    }

    FEInterpolation *giveInterpolation() const override { return const_cast< BSplineInterpolation * >(& this->interpolation); }
    Element *giveElement() override { return this; }
    void giveDofManDofIDMask(int inode, IntArray &answer) const override {
        PlaneStressStructuralElementEvaluator :: giveDofManDofIDMask(inode, answer);
    }
    int computeNumberOfDofs() override { return numberOfDofMans * 2; }
    void updateInternalState(TimeStep *tStep) override { PlaneStressStructuralElementEvaluator :: updateInternalState(tStep); }
    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_BsplinePlaneStressElement_Name; }
    const char *giveClassName() const override { return "BsplinePlaneStressElement"; }

protected:
    int giveNsd() override { return 2; }
};


class NURBSPlaneStressElement : public IGAElement, public PlaneStressStructuralElementEvaluator
{
protected:
    NURBSInterpolation interpolation;

public:
    NURBSPlaneStressElement(int n, Domain * aDomain);

    void initializeFrom(InputRecord &ir) override;
    int checkConsistency() override;

    void giveCharacteristicMatrix(FloatMatrix &answer, CharType mtrx, TimeStep *tStep) override {
        PlaneStressStructuralElementEvaluator :: giveCharacteristicMatrix(answer, mtrx, tStep);
    }
    void giveCharacteristicVector(FloatArray &answer, CharType type, ValueModeType mode, TimeStep *t) override {
        PlaneStressStructuralElementEvaluator :: giveCharacteristicVector(answer, type, mode, t);
    }

    FEInterpolation *giveInterpolation() const override { return const_cast< NURBSInterpolation * >(& this->interpolation); }
    Element *giveElement() override { return this; }
    void giveDofManDofIDMask(int inode, IntArray &answer) const override {
        PlaneStressStructuralElementEvaluator :: giveDofManDofIDMask(inode, answer);
    }
    int computeNumberOfDofs() override { return numberOfDofMans * 2; }
    void updateInternalState(TimeStep *tStep) override { PlaneStressStructuralElementEvaluator :: updateInternalState(tStep); }
    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_NURBSPlaneStressElement_Name; }
    const char *giveClassName() const override { return "NURBSPlaneStressElement"; }

protected:
    int giveNsd() override { return 2; }
};



class TSplinePlaneStressElement : public IGATSplineElement, public PlaneStressStructuralElementEvaluator
{
protected:
    TSplineInterpolation interpolation;

public:
    TSplinePlaneStressElement(int n, Domain * aDomain);

    void initializeFrom(InputRecord &ir) override {
        IGATSplineElement :: initializeFrom(ir);
        //PlaneStressStructuralElementEvaluator::initializeFrom(ir);
    }

    void giveCharacteristicMatrix(FloatMatrix &answer, CharType mtrx, TimeStep *tStep) override {
        PlaneStressStructuralElementEvaluator :: giveCharacteristicMatrix(answer, mtrx, tStep);
    }
    void giveCharacteristicVector(FloatArray &answer, CharType type, ValueModeType mode, TimeStep *t) override {
        PlaneStressStructuralElementEvaluator :: giveCharacteristicVector(answer, type, mode, t);
    }

    FEInterpolation *giveInterpolation() const override { return const_cast< TSplineInterpolation * >(& this->interpolation); }
    Element *giveElement() override { return this; }
    void giveDofManDofIDMask(int inode, IntArray &answer) const override {
        PlaneStressStructuralElementEvaluator :: giveDofManDofIDMask(inode, answer);
    }
    int computeNumberOfDofs() override { return numberOfDofMans * 2; }
    void updateInternalState(TimeStep *tStep) override { PlaneStressStructuralElementEvaluator :: updateInternalState(tStep); }
    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_TSplinePlaneStressElement_Name; }
    const char *giveClassName() const override { return "TSplinePlaneStressElement"; }

protected:
    int giveNsd() override { return 2; }
};


class NURBSSpace3dElement : public IGAElement, public Space3dStructuralElementEvaluator
{
protected:
    NURBSInterpolation interpolation;

public:
    NURBSSpace3dElement(int n, Domain * aDomain);

    void initializeFrom(InputRecord &ir) override;
    int checkConsistency() override;

    void giveCharacteristicMatrix(FloatMatrix &answer, CharType mtrx, TimeStep *tStep) override {
        Space3dStructuralElementEvaluator :: giveCharacteristicMatrix(answer, mtrx, tStep);
    }
    void giveCharacteristicVector(FloatArray &answer, CharType type, ValueModeType mode, TimeStep *t) override {
        Space3dStructuralElementEvaluator :: giveCharacteristicVector(answer, type, mode, t);
    }

    FEInterpolation *giveInterpolation() const override { return const_cast< NURBSInterpolation * >(& this->interpolation); }
    Element *giveElement() override { return this; }

    void giveDofManDofIDMask(int inode, IntArray &answer) const override {
        Space3dStructuralElementEvaluator :: giveDofManDofIDMask(inode, answer);
    }
    int computeNumberOfDofs() override { return numberOfDofMans * 3; }
    void updateInternalState(TimeStep *tStep) override { Space3dStructuralElementEvaluator :: updateInternalState(tStep); }
    // definition & identification
    const char *giveInputRecordName() const override { return _IFT_NURBSSpace3dElement_Name; }
    const char *giveClassName() const override { return "NURBSSpace3dElement"; }

protected:
    int giveNsd() override { return 3; }
};
} // end namespace oofem
#endif //igaelements_h
