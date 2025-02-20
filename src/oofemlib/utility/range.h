// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef range_h
#define range_h

//#include <iosfwd>
#include <ostream>

#include "oofemcfg.h"

namespace oofem {
/**
 * Class Range is an abstraction for interval of integer numbers. It is described using its start and end values of interval
 * it represents. The interval is defined to represent all values between start and end values, including start and end values.
 * Function for testing if number is in interval is provided. Used by OutputManager
 * to efficiently maintain intervals.
 */
class OOFEM_EXPORT Range
{
protected:
    /// Interval start value.
    int startIndx;
    /// Interval end value.
    int endIndx;

public:
    /// Constructor. Creates Range containing only given single number
    Range(int indx) {
        startIndx = endIndx = indx;
    }
    /// Constructor. Creates range <li, hi>
    Range(int li, int hi) {
        startIndx = li;
        endIndx = hi;
    }
    /// Empty range constructor.
    Range() {
        startIndx = 0;
        endIndx = -1;
    }

    /// Returns the start index (inclusive).
    int giveStart() { return startIndx; }
    /// Returns the end index (inclusive).
    int giveEnd() { return endIndx; }

    /// Tests if number is in range.
    bool test(int i) { return ( i >= startIndx ) && ( i <= endIndx ); }

    friend std :: ostream &operator << ( std :: ostream & out, const Range & r ) {
        return out << r.startIndx << " " << r.endIndx;
    }
};
} // end namespace oofem
#endif // range_h
