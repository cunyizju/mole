// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef problemmode_h
#define problemmode_h

namespace oofem {
enum problemMode {
    _processor,
    _postProcessor
};

/// Corresponds to macro- and micro-problem in multiscale simulations.
enum problemScale {
    macroScale,
    microScale
};
} // end namespace oofem
#endif // problemmode_h
