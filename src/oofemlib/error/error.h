// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

//   ************************************
//   *** Error macros                 ***
//   ************************************

#ifndef error_h
#define error_h

#include "input/logger.h"

#include <string>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>


namespace oofem {
/** Cause oofem program termination by calling exit. */
#define OOFEM_EXIT(code) \
    oofem_logger.printStatistics(); \
    fprintf(stderr, "oofem exit code %d\n", code); \
    exit(code);


class RuntimeException : public std::exception
{
public:
    std::string msg;

    RuntimeException(const char* _func, const char* _file, int _line, const char *format, ...);
    const char* what() const noexcept override;
};


/**
 * Macros for printing errors.
 * This macro can be used only within classes that implement errorInfo function.
 */
//@{
#define OOFEM_FATAL(...) { throw RuntimeException(__func__, __FILE__, __LINE__, __VA_ARGS__);}
#define OOFEM_ERROR(...) { throw RuntimeException(__func__, __FILE__, __LINE__, __VA_ARGS__);}
#define OOFEM_WARNING(...) oofem_logger.writeELogMsg(Logger :: LOG_LEVEL_WARNING, errorInfo(__func__).c_str(), __FILE__, __LINE__, __VA_ARGS__)
#define OOFEM_SERROR(...) { oofem_logger.writeELogMsg(Logger :: LOG_LEVEL_ERROR, __func__, __FILE__, __LINE__, __VA_ARGS__); OOFEM_EXIT(1); }
#define OOFEM_SWARNING(...) oofem_logger.writeELogMsg(Logger :: LOG_LEVEL_WARNING, __func__, __FILE__, __LINE__, __VA_ARGS__)
//@}

OOFEM_EXPORT std::string errorInfo(const char *func);

} // end namespace oofem
#endif // error_h
