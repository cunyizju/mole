// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "fields/intvarfield.h"
#include "engng/classfactory.h"
#include "dofman/dofmanager.h"
#include "material/materialmappingalgorithm.h"

namespace oofem {
InternalVariableField :: InternalVariableField(InternalStateType ist, FieldType ft, MaterialMappingAlgorithmType mma_type, Domain *d) :
    Field(ft),
    mma(classFactory.createMaterialMappingAlgorithm(mma_type)),
    type(ist),
    domain(d)
{}

int
InternalVariableField :: evaluateAt(FloatArray &answer, const FloatArray &coords, ValueModeType mode, TimeStep *tStep)
{
    IntArray types;
    types.at(1) = this->type;
    /// Use MaterialMappingAlgorithm classes to do the job
    Set eset(0, domain);
    eset.addAllElements();
    this->mma->__init(domain, types, coords, eset, tStep);
    this->mma->__mapVariable(answer, coords, this->type, tStep);

    return 0; // ok
}

int
InternalVariableField :: evaluateAt(FloatArray &answer, DofManager *dman, ValueModeType mode, TimeStep *tStep)
{
    return this->evaluateAt(answer, dman->giveCoordinates(), mode, tStep);
}

void
InternalVariableField :: saveContext(DataStream &stream)
{}

void
InternalVariableField :: restoreContext(DataStream &stream)
{}
} // end namespace oofem
