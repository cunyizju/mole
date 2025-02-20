// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li
#ifndef PRESCRIBEDMEAN_H
#define PRESCRIBEDMEAN_H

#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <memory>

#include "bc/activebc.h"
#include "dofman/node.h"
#include "bc/boundaryload.h"

namespace oofem
{

///@name Input fields for PrescribedMean
//@{
#define _IFT_PrescribedMean_Name "prescribedmean"
#define _IFT_PrescribedMean_DofID "dofid"
#define _IFT_PrescribedMean_Mean "mean"
#define _IFT_PrescribedMean_Edge "edge"

//@}

class OOFEM_EXPORT PrescribedMean : public ActiveBoundaryCondition
{
private:
    std::unique_ptr<Node> lambdaDman;

    double c;

    static double domainSize;

    int dofid;

    void computeDomainSize();

    bool elementEdges;

    IntArray elements;
    IntArray sides;
    IntArray lambdaIDs;

public:
    PrescribedMean(int n, Domain * d) : ActiveBoundaryCondition(n, d), lambdaDman(std::make_unique<Node>(0, this->domain) ) {}
    virtual ~PrescribedMean() {}

    void initializeFrom(InputRecord &ir) override;

    void assemble(SparseMtrx &answer, TimeStep *tStep, CharType type,
                  const UnknownNumberingScheme &r_s, const UnknownNumberingScheme &c_s, double scale = 1.0, void* lock=nullptr) override;

    void assembleVector(FloatArray &answer, TimeStep *tStep,
                        CharType type, ValueModeType mode,
                        const UnknownNumberingScheme &s, FloatArray *eNorm=nullptr, void* lock=nullptr) override;

    void giveInternalForcesVector(FloatArray &answer, TimeStep *tStep,
                                  CharType type, ValueModeType mode,
                                  const UnknownNumberingScheme &s, FloatArray *eNorm=nullptr, 
                                  void* lock=nullptr);

    void giveExternalForcesVector(FloatArray &answer, TimeStep *tStep,
                                  CharType type, ValueModeType mode,
                                  const UnknownNumberingScheme &s,
                                  void* lock=nullptr);

    int giveNumberOfInternalDofManagers() override { return 1; }

    DofManager *giveInternalDofManager(int i) override { return lambdaDman.get(); }

    const char *giveClassName() const override { return "PrescribedMean"; }
    const char *giveInputRecordName() const override { return _IFT_PrescribedMean_Name; }
};

}

#endif // PRESCRIBEDMEAN_H
