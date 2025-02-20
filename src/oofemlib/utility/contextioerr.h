// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef contextioerr_h
#define contextioerr_h

#include <exception>

#include "oofemcfg.h"
#include "utility/contextioresulttype.h"

namespace oofem {
/**
 * Context IO exception class
 * @todo Document more.
 */
class OOFEM_EXPORT ContextIOERR : public std::exception
{
    contextIOResultType error;
    const char *msg, *file;
    int line;
    std::string full_message;

public:
    ContextIOERR(contextIOResultType e, const char *file, int line);
    ContextIOERR(contextIOResultType e, const char *msg, const char *file, int line);

    const char* what() const noexcept override { return this->full_message.c_str(); }
    void print();
};

#define THROW_CIOERR(e) throw ContextIOERR(e, __FILE__, __LINE__);
#define THROW_CIOERRM(e, m) throw ContextIOERR(e, m, __FILE__, __LINE__);
} // end namespace oofem
#endif // contextioerr_h
