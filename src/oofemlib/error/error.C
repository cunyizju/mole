// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "error/error.h"

#include <string>
#include <cstdarg>



namespace oofem {



std::string errorInfo(const char *func)
{
    return std :: string(func);
}

RuntimeException::RuntimeException(const char * _func, const char * _file, int _line, const char *format, ...)
{
    char stream [500];
    va_list args;
    va_start(args, format);
    vsprintf(stream, format, args);
    va_end(args);
    msg = stream;

    sprintf(stream, "\nIn %s (%s:%d)", _func, _file, _line);
    msg += stream;
}

const char* RuntimeException::what() const noexcept
{ 
    return msg.c_str();
}

} // end namespace oofem
