// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef qc_fullsolveddomain_h
#define qc_fullsolveddomain_h

#include "math/floatarray.h"
#include "input/element.h"
#include "dofman/node.h"

#define _IFT_FullSolvedDomain_nodes "fsd_n"
#define _IFT_FullSolvedDomain_elements "fsd_e"
#define _IFT_FullSolvedDomain_radius "fsd_r"
#define _IFT_FullSolvedDomain_box "fsd_b"


//#define _IFT_QCFullsolveddomain_Name "QCFullsolveddomain"

namespace oofem {

/**
 * Information about fullsolved domain in CQ simulation.
 * 
 */
class QCFullsolveddomain 

{
protected:
    FloatArray FullSolvedDomainNodes;
    FloatArray FullSolvedDomainElements;
    FloatArray FullSolvedDomainRadius;
    FloatArray FullSolvedDomainBox;

public:
    QCFullsolveddomain();
    virtual ~QCFullsolveddomain();

    virtual void initializeFrom(InputRecord &ir);

    virtual void updateYourself();

    virtual bool isNodeInside(Node *n);


    // identification
    //virtual const char *giveInputRecordName() const { return _IFT_QCFullsolveddomain_Name; }
    virtual const char *giveClassName() const { return "QCFullsolveddomain"; }



};
} // end namespace oofem
#endif // qc_fullsolveddomain_h
