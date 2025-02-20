// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef intvarfield_h
#define intvarfield_h

#include "fields/field.h"
#include "material/materialmappingalgorithmtype.h"
#include "input/internalstatetype.h"
#include "material/materialmappingalgorithm.h"

namespace oofem {
class Domain;
class MaterialMappingAlgorithm;

/**
 * Abstract class representing a field of an internal variable.
 * Field represent the spatial distribution of certain variable and is able to
 * evaluate its value at any point of interest.
 * The field is usually associated to the specific domain.
 *
 * It uses MaterialMappingAlgorithm interface to perform interpolation.
 * Note, that some classes implementing MaterialMappingAlgorithm may require
 * that elements implement corresponding interface.
 */
class OOFEM_EXPORT InternalVariableField : public Field
{
protected:
    /// Material mapping algorithm used.
    std::unique_ptr<MaterialMappingAlgorithm> mma;
    /// InternalStateType.
    InternalStateType type;
    /// Source domain.
    Domain *domain;

public:
    /**
     * Constructor. Creates a internal variable field of given type associated to given domain.
     * @param ist Physical meaning of field.
     * @param b Field type.
     * @param mma_type Algorithm used to map materials.
     * @param d Domain which field belongs to.
     */
    InternalVariableField(InternalStateType ist, FieldType b, MaterialMappingAlgorithmType mma_type, Domain * d);
    virtual ~InternalVariableField() {}

    int evaluateAt(FloatArray &answer, const FloatArray &coords, ValueModeType mode, TimeStep *tStep) override;
    int evaluateAt(FloatArray &answer, DofManager *dman, ValueModeType mode, TimeStep *tStep) override;

    void saveContext(DataStream &stream) override;
    void restoreContext(DataStream &stream) override;
    const char *giveClassName() const override { return "InternalVariableField"; }
};
} // end namespace oofem
#endif // intvarfield_h
