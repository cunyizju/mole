// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef maskedprimaryfield_h
#define maskedprimaryfield_h

#include "fields/primaryfield.h"
#include "math/floatarray.h"
#include "math/intarray.h"
#include "input/valuemodetype.h"
#include "utility/contextioresulttype.h"
#include "utility/contextmode.h"

#include <memory>

namespace oofem {
/**
 * Abstract class representing subset of DOFs (identified by DofId mask) of primary field.
 * As the PrimaryField stores the state directly in solution vectors that are usually directly
 * updated by EngngModel, it may contain a mix of different fields (this is especially true for
 * strongly coupled problems). Then masked primary field can be used to select only certain DOFs
 * (based on DofID) from its master PrimaryField.
 */
class OOFEM_EXPORT MaskedPrimaryField : public Field
{
protected:
    PrimaryField *master;
    IntArray mask;

public:
    MaskedPrimaryField(FieldType b, PrimaryField * m, IntArray dofIdMask) : Field(b),
        master(m),
        mask(std::move(dofIdMask))
    { }

    int evaluateAt(FloatArray &answer, const FloatArray &coords, ValueModeType mode, TimeStep *tStep) override;
    int evaluateAt(FloatArray &answer, DofManager *dman, ValueModeType mode, TimeStep *tStep) override;

    void restoreContext(DataStream &stream) override { }
    void saveContext(DataStream &stream) override { }

    const char *giveClassName() const override { return "MaskedPrimaryField"; }
};
} // end namespace oofem
#endif // maskedprimaryfield_h
