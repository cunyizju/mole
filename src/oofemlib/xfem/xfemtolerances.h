// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef XFEMTOLERANCES_H_
#define XFEMTOLERANCES_H_


namespace oofem {


  /**
 * Provides tolerances for the XFEM module.
 * @author Erik Svenning
 * @date Jul 16, 2014
 */

class XfemTolerances
{
protected:
    /// Characteristic element length
    static double sCharElLength;

    /// Loose relative length tolerance
#define sRelLengthTolLoose 1.0e-3;

    /// Tight relative length tolerance
#define sRelLengthTolTight 1.0e-6;


    /// Loose dimensionless tolerance (e.g. natural coordinates)
#define sRelDimensionlessTolLoose 1.0e-3;

    /// Tight dimensionless tolerance (e.g. natural coordinates)
#define sRelDimensionlessTolTight 1.0e-6;

    /// Approximate zero
#define sApproxZero 1.0e-12;

private:
    XfemTolerances()
    {}

public:

    static void setCharacteristicElementLength(double iCharElLength);
    static double giveCharacteristicElementLength() { return sCharElLength; }

    static double giveRelLengthTolLoose();
    static double giveRelLengthTolTight();

    static double giveDimensionlessTolLoose() { return sRelDimensionlessTolLoose; }
    static double giveDimensionlessTolTight() { return sRelDimensionlessTolTight; }

    static double giveApproxZero() { return sApproxZero; }
};

} // end namespace oofem
#endif /* XFEMTOLERANCES_H_ */
