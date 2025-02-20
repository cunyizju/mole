// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef contextioresulttype_h
#define contextioresulttype_h

namespace oofem {
enum contextIOResultType {
    CIO_OK = 0,     ///< OK.
    CIO_BADVERSION, ///< Incompatible context file.
    CIO_BADOBJ,     ///< Bad object passed.
    CIO_IOERR       ///< General IO error.
};
} // end namespace oofem
#endif // contextioresulttype_h
