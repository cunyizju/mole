// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef XFEMSOLVERINTERFACE_H_
#define XFEMSOLVERINTERFACE_H_

namespace oofem {

class TimeStep;
class StructuralEngngModel;
class StaticStructural;
class FloatArray;

/**
 * Provides extra solver functionality needed for XFEM.
 * @author Erik Svenning
 */
class XfemSolverInterface {
public:
    XfemSolverInterface();
    virtual ~XfemSolverInterface() { }

    void propagateXfemInterfaces(TimeStep *tStep, StructuralEngngModel &ioEngngModel, bool iRecomputeStepAfterCrackProp);

protected:
    bool mNeedsVariableMapping;
};

} /* namespace oofem */

#endif /* XFEMSOLVERINTERFACE_H_ */
