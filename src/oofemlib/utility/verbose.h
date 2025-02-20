// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

/**
 * @file verbose.h
 *
 * Initializes the variable VERBOSE, in order to get a few intermediate
 * messages on screen: beginning and end of every time step, assembly of
 * every element, assembly of every node's load vector.
 *
 * Initializes the variable DETAILED_REPORT, in order to get a very detailed
 * messages on screen.
 *
 * Initializes the variable TIME_REPORT, in order to get a detailed
 * time summary of solution (assembly time, factorization time, time per solution step, etc.).
 */

#ifndef verbose_h
#define verbose_h

namespace oofem {
#define VERBOSE             // please activate or de-activate this line

#define VERBOSE_PRINTS(str, str1) OOFEM_LOG_INFO("%-30s %6s\n", str, str1);
#define VERBOSE_PRINT0(str, number) OOFEM_LOG_DEBUG("%-30s %6d\n", str, number);

#define TIME_REPORT        // please activate or de-activate this line

#ifndef DETAILED_REPORT
//#define DETAILED_REPORT  // please activate or de-activate this line
//#define VERBOSE          // please activate or de-activate this line
 #define TIME_REPORT       // please activate or de-activate this line
#endif
} // end namespace oofem
#endif // verbose_h
