// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef qcmaterialextensioninterface_h
#define qcmaterialextensioninterface_h

#include "utility/interface.h"


namespace oofem {
class FloatMatrix;
class GaussPoint;
class TimeStep;



/**
 * Material interface for gradient material models.
 */
class QCMaterialExtensionInterface : public Interface
{
public:
    /**
     * Constructor. Creates material with given number, belonging to given domain.
     * @param d Domain to which new material will belong.
     */
    QCMaterialExtensionInterface() {}
    /// Destructor.
    virtual ~QCMaterialExtensionInterface() { }
    //
    virtual double giveQcElasticParamneter() = 0;
    virtual double giveQcPlasticParamneter() = 0;
};
}
#endif
