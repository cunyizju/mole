// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef PRESCRIBEDGRADIENTBCNEUMANN_H_
#define PRESCRIBEDGRADIENTBCNEUMANN_H_

#include "bc/prescribedgradienthomogenization.h"
#include "bc/activebc.h"

#include <memory>

#define _IFT_PrescribedGradientBCNeumann_Name   "prescribedgradientbcneumann"

namespace oofem {
class Node;
class Element;
/**
 * Imposes a prescribed gradient weakly on the boundary
 * with a Neumann boundary condition.
 *
 * @author Erik Svenning
 * @author Mikael Öhman
 */
class OOFEM_EXPORT PrescribedGradientBCNeumann : public ActiveBoundaryCondition, public PrescribedGradientHomogenization
{
public:
    PrescribedGradientBCNeumann(int n, Domain *d);
    virtual ~PrescribedGradientBCNeumann();

    int giveNumberOfInternalDofManagers() override { return 1; }
    DofManager *giveInternalDofManager(int i) override;

    void initializeFrom(InputRecord &ir) override;
    void giveInputRecord(DynamicInputRecord &input) override;

    bcType giveType() const override { return UnknownBT; }

    void scale(double s) override;

    void assembleVector(FloatArray &answer, TimeStep *tStep,
                        CharType type, ValueModeType mode,
                        const UnknownNumberingScheme &s, FloatArray *eNorm=nullptr, void* lock=nullptr) override;

    void assemble(SparseMtrx &answer, TimeStep *tStep,
                  CharType type, const UnknownNumberingScheme &r_s, const UnknownNumberingScheme &c_s, 
                  double scale = 1.0, void* lock=nullptr) override;

    void giveLocationArrays(std :: vector< IntArray > &rows, std :: vector< IntArray > &cols, CharType type,
                            const UnknownNumberingScheme &r_s, const UnknownNumberingScheme &c_s) override;

    const char *giveClassName() const override { return "PrescribedGradientBCNeumann"; }
    const char *giveInputRecordName() const override { return _IFT_PrescribedGradientBCNeumann_Name; }

    void computeField(FloatArray &sigma, TimeStep *tStep) override;
    void computeTangent(FloatMatrix &tangent, TimeStep *tStep) override;

    void giveStressLocationArray(IntArray &oCols, const UnknownNumberingScheme &r_s);

protected:
    /// DOF-manager containing the unknown homogenized stress.
    std :: unique_ptr< Node > mpSigmaHom;
    IntArray mSigmaIds;

    /// Help function that integrates the tangent contribution from a single element boundary.
    void integrateTangent(FloatMatrix &oTangent, Element *e, int iBndIndex);
};
} /* namespace oofem */

#endif /* PRESCRIBEDGRADIENTBCNEUMANN_H_ */
