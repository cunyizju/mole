// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef contactdefinitionnode2node_h
#define contactdefinitionnode2node_h

#include "contact/contactdefinition.h"


///@name Input fields for _IFT_ContactDefinitionNode2Node
//@{
#define _IFT_ContactDefinitionNode2Node_Name "cdef_node2nodep"
#define _IFT_ContactDefinitionNode2Node_MasterNodes "masternodes"
#define _IFT_ContactDefinitionNode2Node_SlaveNodes "slavenodes"
#define _IFT_ContactDefinitionNode2Node_PenaltyN "penaltyn"

#define _IFT_ContactDefinitionNode2NodeL_Name "cdef_node2nodeL"
//@}

namespace oofem {
class Domain;
class ContactManager;
class ContactObject;
class ContactElement;

class ContactMaterial; // write this


/**
 * This class manages a particular contact definition. 
 * This keeps track of the discretization, how the contact constraints are enforced and so on
 *
 * @author Jim Brouzoulis
 */
class OOFEM_EXPORT ContactDefinitionNode2Node : public ContactDefinition
{
private:
    double epsN;
    double epsT; // these should be 'contactmaterial' par

public:

    /// Constructor.
    ContactDefinitionNode2Node(ContactManager *cMan);
    /// Destructor.
    virtual ~ContactDefinitionNode2Node(){};

    void initializeFrom(InputRecord &ir) override;

    const char *giveClassName() const override { return "ContactDefinitionNode2Node"; }
    virtual const char *giveInputRecordName() const { return _IFT_ContactDefinitionNode2Node_Name; } 
};


/**
 * This class manages a none to node contact definition with enforcement using Lagrange multipliers.
 *
 * @author Jim Brouzoulis
 */
class OOFEM_EXPORT ContactDefinitionNode2NodeL : public ContactDefinitionNode2Node
{

public:

    /// Constructor.
    ContactDefinitionNode2NodeL(ContactManager *cMan);
    /// Destructor.
    virtual ~ContactDefinitionNode2NodeL(){};

    void initializeFrom(InputRecord &ir) override;

    const char *giveClassName() const override { return "ContactDefinitionNode2NodeL"; }
    const char *giveInputRecordName() const override { return _IFT_ContactDefinitionNode2NodeL_Name; }
};


} // end namespace oofem
#endif // contactdefinitionnode2node_h
