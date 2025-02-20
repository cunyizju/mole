// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef internalstatevaluetype_h
#define internalstatevaluetype_h

namespace oofem {
/// Determines the type of internal variable.
enum InternalStateValueType {
    ISVT_UNDEFINED, ///< Undefined.
    ISVT_SCALAR,    ///< Scalar.
    ISVT_VECTOR,    ///< Vector.
    ISVT_TENSOR_S3, ///< Symmetric 3x3 tensor.
    ISVT_TENSOR_S3E, ///< symmetric 3x3 tensor, packed with off diagonal components multiplied by 2 (engineering strain vector, for example).
    ISVT_TENSOR_G    ///< General tensor.
};
} // end namespace oofem
#endif // internalstatevaluetype_h
