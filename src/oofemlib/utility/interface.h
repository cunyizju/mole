// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef interface_h
#define interface_h

#include "oofemcfg.h"

#include <string>

namespace oofem {
/**
 * Class Interface. Interface class is abstract base class.
 * Its role is to provide a base class for all interfaces.
 * The interface represent some well defined additional or optional ability or support, which is provided by some class.
 * The services and attributes are encapsulated in particular interface class,
 * which is required to be derived from base Interface class.
 * The class which wants to implement interface will simply inherits corresponding interface class.
 * Each base class, whose derived classes are assumed to implement some interfaces, will declare
 * the getInterface(InterfaceType) method. This method must be implemented by each derived class implementing
 * some interface and returns pointer to corresponding interface. This service provides general way, how to
 * access particular interface of given class.
 *
 * The interface concept can be illustrated on following simple example.
 * Let's have an error estimator, represented by corresponding class. This estimator requires some special
 * services at the element level. Because typically not all elements provide support for this error estimator,
 * it would be wasting of space to declare required services in general element base class.
 * (This will lead to big and extremely general interface declared by base class,
 * with only some services compulsory and with many optional services. This can lead to confusion and very unclear structure).
 * Suggested remedy is to declare corresponding interface to error estimator. The interface
 * should declare all services needed by error estimator. Particular elements, which would like to provide support for
 * error estimator, simply derive itself from corresponding interface and implement services from interface.
 * If error estimator wants to access its interface, it asks element for pointer to its corresponding interface
 * (getInterface method). Once interface is returned, error estimator can use its services to compute response.
 *
 * Some difficulty may arise, when implementing general interface service, requiring general services
 * from class implementing interface. Then simple "trick" can be made:
 * The interface should declare service giveClass(), which returns the pointer to class which implements interface.
 * After having this pointer, the interface general service can use all (but only public) services of class implementing
 * interface. For example some service utilizing the integration over element volume can be formulated as general,
 * but when interface is defined, there is no connection to element implementing it. This connection can be established
 * by declaring virtual giveElement() service, which returns the pointer to element implementing the interface.
 * Then general implementation at interface class level can be made, using base Element class public services.
 * Then all element classes implementing the interface should overload only giveElement service, but not the whole
 * service with integration.
 *
 * Interfaces provide a way, how to organize or group optional or function-specific services into
 * clearly defined units (well structured code) which can be selectively implemented by some classes.
 */

class OOFEM_EXPORT Interface
{
public:
    /// Constructor.
    Interface() { }
    virtual ~Interface() { }
    virtual const char *giveClassName() const = 0;
};
} // end namespace oofem
#endif // interface_h
