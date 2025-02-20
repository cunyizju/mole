// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef smoothednodalintvarfield_h
#define smoothednodalintvarfield_h

#include "fields/field.h"
#include "nodalrecovery/zznodalrecoverymodel.h"

namespace oofem {
class Domain;

/**
 * Class representing a field of an internal variable smoothed from integration points
 * into nodes. It is relying on NodalRecoveryModel to smooth the data.
 */
class OOFEM_EXPORT SmoothedNodalInternalVariableField : public Field
{
protected:
    /// Smoother type
    NodalRecoveryModel :: NodalRecoveryModelType stype;
    /// Smoother
    std::unique_ptr<NodalRecoveryModel> smoother;
    /// InternalStateType.
    InternalStateType istType;
    /// Source domain.
    Domain *domain;

public:
    /**
     * Constructor. Creates a internal variable field of given type associated to given domain.
     * @param ist Physical meaning of field.
     * @param b Field type.
     * @param st Determines the type of nodal recovery model used.
     * @param d Domain which field belongs to.
     */
    SmoothedNodalInternalVariableField(InternalStateType ist, FieldType b, NodalRecoveryModel :: NodalRecoveryModelType st, Domain * d);
    virtual ~SmoothedNodalInternalVariableField();

    int evaluateAt(FloatArray &answer, const FloatArray &coords, ValueModeType mode, TimeStep *tStep) override;
    int evaluateAt(FloatArray &answer, DofManager *dman, ValueModeType mode, TimeStep *tStep) override;

    InternalStateType giveInternalType() { return istType; }
    void saveContext(DataStream &stream) override;
    void restoreContext(DataStream &stream) override;
    const char *giveClassName() const override { return "SmoothedNodalInternalVariableField"; }
};
} // end namespace oofem
#endif // smoothednodalintvarfieldintvarfield_h
