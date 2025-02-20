// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef space3delementevaluator_h
#define space3delementevaluator_h

#include "sm/Elements/structuralelementevaluator.h"

namespace oofem {
/**
 * General purpose 3d structural element evaluator.
 */
class Space3dStructuralElementEvaluator : public StructuralElementEvaluator
{
public:
    Space3dStructuralElementEvaluator() : StructuralElementEvaluator() { }
    virtual ~Space3dStructuralElementEvaluator() { }

protected:
    /**
     * Assemble interpolation matrix at given IP
     * In case of IGAElements, N is assumed to contain only nonzero interpolation functions
     */
    void computeNMatrixAt(FloatMatrix &answer, GaussPoint *gp) override;
    /**
     * Assembles the strain-displacement matrix of the receiver at given integration point
     * In case of IGAElements, B is assumed to contain only contribution from nonzero interpolation functions
     */
    void computeBMatrixAt(FloatMatrix &answer, GaussPoint *gp) override;
    double computeVolumeAround(GaussPoint *gp) override;
    void computeStressVector(FloatArray &answer, const FloatArray &strain, GaussPoint *gp, TimeStep *tStep) override;
    void computeConstitutiveMatrixAt(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override;
    //void computeConstitutiveMatrix_dPdF_At(FloatMatrix &answer, MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) override;
    void giveDofManDofIDMask(int inode, IntArray &answer) const {
        answer = { D_u, D_v, D_w };
    }
};
} // end namespace oofem
#endif //space3delementevaluator_h
