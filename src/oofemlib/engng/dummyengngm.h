// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef dummyengngm_h
#define dummyengngm_h

#include "engng/engngm.h"
#include "input/inputrecord.h"



#define _IFT_DummyEngngModel_Name "Dummy"

namespace oofem {



/**
 * Dummy Engneering model. Does not solve any problem, but invokes the configured export modules.
 * Usefull for exporting model geometry without solving the problem.
 */
class OOFEM_EXPORT DummyEngngModel : public EngngModel
{
public:

    /**
     * Constructor. Creates Engng model with number i.
     */
    DummyEngngModel(int i, EngngModel * _master = NULL);
    virtual ~DummyEngngModel() {}

    void solveYourselfAt(TimeStep *tStep) override;
    void initializeFrom(InputRecord &ir) override;
    TimeStep * giveNextStep() override;
    const char *giveClassName() const override { return "DummyEngngModel"; }
 

};

} // end namespace oofem
#endif // engngm_h
