// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef util_h
#define util_h

#include "oofemcfg.h"
#include "input/problemmode.h"

#include <memory>
#include <cstdio>

namespace oofem {
class DataReader;
class EngngModel;

/**
 * Helper that prints a stack trace (only available on GCC)
 */
void print_stacktrace(FILE *out = stderr, int skip = 0, unsigned int max_frames = 63);

/**
 * Instanciates the new problem.
 * @param dr DataReader containing the problem data.
 * @param mode Mode determining macro or micro problem.
 * @param master Master problem in case of multiscale computations.
 * @param parallelFlag Determines if the problem should be run in parallel or not.
 * @param contextFlag When set, turns on context output after each step.
 */
OOFEM_EXPORT std::unique_ptr<EngngModel> InstanciateProblem(DataReader &dr, problemMode mode, int contextFlag, EngngModel *master = 0, bool parallelFlag = false);
} // end namespace oofem
#endif // util_h
