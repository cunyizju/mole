// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef fieldmanager_h
#define fieldmanager_h

#include "oofemcfg.h"
#include "fields/field.h"

#include <map>
#include <vector>
#include <memory>

namespace oofem {

class OOFEM_EXPORT FieldManager
{
protected:

    /**
     * Field container. Stores smart pointers to objects (not object themselves)
     * to avoid copying elements and to preserve the use of polymorphic types.
     * The use of shared_ptr is essential here, as some registered fields may be
     * ovned (and maintained) by emodel, some may be cretead on demand and thus
     * managed only by field manager.
     */
    std :: map< FieldType, std :: shared_ptr< Field > >externalFields;

public:
    FieldManager() : externalFields() { }
    ~FieldManager();

    /**
     * Registers the given field (the receiver is not assumed to own given field).
     * The field is registered under given key. Using this key, it can be later accessed.
     */
    void registerField(FieldPtr eField, FieldType key);

    /**
     * Returns the previously registered field under given key; NULL otherwise
     */
    FieldPtr giveField(FieldType key);

    /** Returns true if field is registered under key */
    bool isFieldRegistered(FieldType key);
    /**
     * Unregisters (deletes) the field registered under given key.
     */
    void unregisterField(FieldType key);
    /**
     * Returns list of registered field keys, which can be obtained by calling giveField.
     */
    std::vector<FieldType> giveRegisteredKeys();

};
} // end namespace oofem
#endif // fieldmanager_h
