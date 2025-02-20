// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "sm/Contact/cdefnode2node.h"
#include "sm/Contact/celnode2node.h"
#include "input/domain.h"
#include "engng/classfactory.h"

namespace oofem {
REGISTER_ContactDefinition(ContactDefinitionNode2Node)
REGISTER_ContactDefinition(ContactDefinitionNode2NodeL)


ContactDefinitionNode2Node :: ContactDefinitionNode2Node(ContactManager *cMan) : ContactDefinition(cMan){}
    

void
ContactDefinitionNode2Node :: initializeFrom(InputRecord &ir)
{
    IntArray masterNodes;
    IntArray slaveNodes;
    IR_GIVE_FIELD(ir, masterNodes, _IFT_ContactDefinitionNode2Node_MasterNodes);
    IR_GIVE_FIELD(ir, slaveNodes, _IFT_ContactDefinitionNode2Node_SlaveNodes);
    //this->epsN = 1.0e6;
    //IR_GIVE_OPTIONAL_FIELD(ir, this->epsN, _IFT_ContactDefinitionNode2Node_PenaltyN);
    
    Domain *domain = this->giveContactManager()->giveDomain();
    for( int i = 1; i<= masterNodes.giveSize(); i++ ) {
        ContactElement *master = new Node2NodeContact( domain->giveDofManager(masterNodes.at(i)),
                                                       domain->giveDofManager(slaveNodes.at(i)));

        this->addContactElement(master);
    }
}


// Same version but with Lagrange multipliers
ContactDefinitionNode2NodeL :: ContactDefinitionNode2NodeL(ContactManager *cMan) : ContactDefinitionNode2Node(cMan)
{
    this->setNumberOfConstraintEqToAdd(1);
}
    

void
ContactDefinitionNode2NodeL :: initializeFrom(InputRecord &ir)
{
    IntArray masterNodes;
    IntArray slaveNodes;
    IR_GIVE_FIELD(ir, masterNodes, _IFT_ContactDefinitionNode2Node_MasterNodes);
    IR_GIVE_FIELD(ir, slaveNodes, _IFT_ContactDefinitionNode2Node_SlaveNodes);
    
    Domain *domain = this->giveContactManager()->giveDomain();
    for( int i = 1; i<= masterNodes.giveSize(); i++ ) {
        ContactElement *master = new Node2NodeContactL( domain->giveDofManager(masterNodes.at(i)),
                                                        domain->giveDofManager(slaveNodes.at(i)) );

        this->addContactElement(master);
    }
}

}
