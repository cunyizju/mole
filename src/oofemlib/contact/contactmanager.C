// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "contact/contactmanager.h"
#include "contact/contactdefinition.h"
#include "engng/classfactory.h"
#include "math/numericalcmpn.h"
#include "error/error.h"

namespace oofem {
REGISTER_ContactManager(ContactManager)

ContactManager :: ContactManager(Domain *domain)
{
    this->domain = domain;
    numberOfContactDefinitions = -1;
}

ContactManager :: ~ContactManager()
{
}


void
ContactManager :: initializeFrom(InputRecord &ir)
{
    this->numberOfContactDefinitions = 0;
    IR_GIVE_FIELD(ir, this->numberOfContactDefinitions, _IFT_ContactManager_NumberOfContactDefinitions);
    
    this->contactDefinitionList.resize(this->numberOfContactDefinitions);
#if 0
    for ( int i = 1; i <= numberOfContactDefinitions; i++ ) {
        std::string name;
        ir.giveRecordKeywordField(name);
        this->contactDefinitionList[i-1] = new ContactDefinition(this);
        this->contactDefinitionList[i-1] = classFactory.createContactDefinition( name.c_str(), this );
    }
#endif
}


int 
ContactManager :: instanciateYourself(DataReader &dr)
{
    std :: string name;

    // Create and instantiate contact definitions
    for ( int i = 1; i <= this->giveNumberOfContactDefinitions(); i++ ) {
        auto &ir = dr.giveInputRecord(DataReader :: IR_contactDefRec, i);
        ir.giveRecordKeywordField(name);
        this->contactDefinitionList[i-1] = classFactory.createContactDefinition( name.c_str(), this );
        if ( this->contactDefinitionList[i-1] ) {
            this->contactDefinitionList[i-1]->initializeFrom(ir);
            this->contactDefinitionList[i-1]->instanciateYourself(dr);
        } else {
            OOFEM_ERROR("Failed to create contact definition (%s)", name.c_str() );
        }
    }

    return 1;
}


void 
ContactManager :: assembleVectorFromContacts(FloatArray &answer, TimeStep *tStep, CharType type, ValueModeType mode,
                                const UnknownNumberingScheme &s, Domain *domain, FloatArray *eNorms)
{
    if ( type == InternalForcesVector) {
        //printf("\n Add forces due to contact... \n");
        for ( auto &cDef : contactDefinitionList ) {
            cDef->computeContactForces(answer, tStep, mode, s, domain, eNorms);
        }
    }
}


void 
ContactManager :: assembleTangentFromContacts(SparseMtrx &answer, TimeStep *tStep,
                      CharType type, const UnknownNumberingScheme &r_s, const UnknownNumberingScheme &c_s) 
{
    if ( type == TangentStiffnessMatrix ) {
        //printf("\n Add tangents due to contact... \n");
        for ( auto &cDef : contactDefinitionList ) {
            cDef->computeContactTangent(answer, tStep, r_s, c_s);
        }
    }
}


void
ContactManager :: createContactDofs()
{
    // Creates new dofs contacts and appends them to the dof managers
    for ( auto &cDef : contactDefinitionList ) {
        cDef->createContactDofs();
    }
}

}
