// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef errorestimatortype_h
#define errorestimatortype_h

namespace oofem {
/**
 * Determines the type of error estimator.
 */
enum ErrorEstimatorType {
    EET_SEI,         ///< Scalar Error Indicator.
    EET_ZZEE,        ///< Zienkiewicz-Zhu EE.
    EET_CZZSI,       ///< Combined ZZ and ScalarIndicator EE.
    EET_HEE,         ///< Huerta EE.
    EET_MeshQuality, ///< Mesh quality/error estimator.
};
} // end namespace oofem
#endif // errorestimatortype_h
