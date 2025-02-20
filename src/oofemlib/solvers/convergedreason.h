// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef convergedreason_h
#define convergedreason_h
namespace oofem {
  /**
   * Typedef to determine reason for a  solver to have converged or diverged
   */
  typedef enum {
    CR_UNKNOWN                             = 0,
    /* converged */
    CR_CONVERGED                           = 1,
    /* diverged */
    CR_DIVERGED_ITS                        = -1,
    CR_DIVERGED_TOL                        = -2,
    /* failed */
    CR_FAILED                              = -99
  } ConvergedReason;
} // end namespace oofem
#endif // convergedreason_h
