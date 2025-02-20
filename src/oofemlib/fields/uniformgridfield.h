// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef uniformgridfield_h
#define uniformgridfield_h

#include "fields/field.h"
#include "math/floatarray.h"
#include "math/intarray.h"

namespace oofem {
/**
 * Field defined by values in uniform grid nodes, with linear interpolation for points inside the grid, or interpolation for the closest point within the grid for points outside.
 */
class OOFEM_EXPORT UniformGridField: public Field
{
protected:
    // points defining axis-aligned box for the grid
    FloatArray lo;
    FloatArray hi;
    // number of divisions along each axis (thus, each axis has div[axis]+1 points!)
    IntArray div;
    // values in all grid points, as linear array; its size must be prod(div)
    // the array is c-ordered, the last index varying the fastest (FIXME: check)
    FloatArray values;
    #if 0
        // precompute useful interally used values (such as cell size)
        void precomputeInternal(); 
    #endif

    void xyz2ijk(const FloatArray& xyz, IntArray& ijk, FloatArray& normXyz) const;
    double nodeValue2d(int i, int j);
    double nodeValue3d(int i, int j, int k);
public:
    /**
     * Constructor. Creates a field, with unspecified field values.
     */
    UniformGridField() : Field(FieldType::FT_Unknown) { }
    virtual ~UniformGridField() { }

    /** Shorthand for defining geometry, with consistency checks. Used primarily from python */
    void setGeometry(const FloatArray& lo_, const FloatArray& hi_, const IntArray& div_);
    /** Accessor for setting nodal values; checks size of the array for correctness. */
    void setValues(const FloatArray& vv);

    int evaluateAt(FloatArray &answer, const FloatArray &coords,
                           ValueModeType mode, TimeStep *tStep) override;

    int evaluateAt(FloatArray &answer, DofManager *dman,
                           ValueModeType mode, TimeStep *tStep) override;

    void saveContext(DataStream &stream) override { }
    void restoreContext(DataStream &stream) override { }

    /// @return Class name of the receiver.
    const char *giveClassName() const override { return "UniformGridField"; }
};
} // end namespace oofem
#endif // field_h
