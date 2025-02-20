// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef contextfilemode_h
#define contextfilemode_h

namespace oofem {
/// Determines the input/output mode of context file.
enum ContextFileMode {
    contextMode_read, ///< Context file is opened for reading.
    contextMode_write, ///< Context mode is opened for writing, if not exist is created, otherwise it will be truncated.
};
} // end namespace oofem
#endif // contextfilemode_h
