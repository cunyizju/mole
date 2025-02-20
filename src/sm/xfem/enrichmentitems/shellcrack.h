// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li
#ifndef SHELLCRACK_H_
#define SHELLCRACK_H_

//#include "xfem/enrichmentitem.h"
#include "xfem/enrichmentitems/crack.h"

#define _IFT_ShellCrack_Name "shellcrack"
#define _IFT_ShellCrack_xiBottom "xibottom"
#define _IFT_ShellCrack_xiTop "xitop"

/**
 * Crack.
 * @author Jim Brouzoulis
 * @date July 29, 2014
 */
namespace oofem {
class XfemManager;
class Domain;
class InputRecord;
class GaussPoint;
class GnuplotExportModule;

class OOFEM_EXPORT ShellCrack : public Crack
{
public:
    ShellCrack(int n, XfemManager *xm, Domain *aDomain);

    const char *giveClassName() const override { return "ShellCrack"; }
    const char *giveInputRecordName() const override { return _IFT_ShellCrack_Name; }
    void initializeFrom(InputRecord &ir) override;

    double xiBottom;
    double xiTop;
};
} // end namespace oofem

#endif /* SHELLCRACK_H_ */
