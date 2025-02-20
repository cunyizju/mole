// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef bcgeomtype_h
#define bcgeomtype_h

namespace oofem {
/// Type representing the geometric character of loading.
enum bcGeomType {
    UnknownBGT,     ///< Unknown type.
    NodalLoadBGT,   ///< Concentrated nodal load.
    BodyLoadBGT,    ///< Distributed body load.
    EdgeLoadBGT,    ///< Distributed edge load.
    SurfaceLoadBGT, ///< Distributed surface load.
    PointLoadBGT,   ///< Concentrated point load (placed anywhere).
    GravityPressureBGT, ///<Pressure due to distributed body load.
};
} // end namespace oofem
#endif // bcgeomtype_h
