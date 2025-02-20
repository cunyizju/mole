// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef contactmanager_h
#define contactmanager_h

#include "input/chartype.h"
#include "input/valuemodetype.h"
#include "oofemcfg.h"
#include "input/datareader.h"
#include "input/inputrecord.h"

#include <vector>
#include <memory>

///@name Input fields for _IFT_ContactManager
//@{
#define _IFT_ContactManager_Name "contactmanager"
#define _IFT_ContactManager_NumberOfContactDefinitions "numcontactdef"

//@}

namespace oofem {
class Domain;
class ContactDefinition;
class TimeStep;
class UnknownNumberingScheme;
class SparseMtrx;
/**
 * This class manages all the contacts in a domain 
 *
 * @author Jim Brouzoulis
 */
class OOFEM_EXPORT ContactManager
{
protected:
    Domain *domain;

private:
    std :: vector< std :: unique_ptr< ContactDefinition > > contactDefinitionList;

public:

    /// Constructor.
    ContactManager(Domain *domain);
    /// Destructor.
    virtual ~ContactManager();

    ContactManager(const ContactManager& src) = delete;
    ContactManager &operator = (const ContactManager &src) = delete;

    void createContactDofs();
    
    /// Initializes receiver according to object description stored in input record.
    virtual void initializeFrom(InputRecord &ir);

    virtual int instanciateYourself(DataReader &dr);
    virtual const char *giveClassName() const { return "ContactManager"; }

    Domain *giveDomain() { return this->domain; }
    int numberOfContactDefinitions;
    ContactDefinition *giveContactDefinition(const int num) { return this->contactDefinitionList[num-1].get(); }
    int giveNumberOfContactDefinitions() const { return (int)contactDefinitionList.size(); }

    
    void assembleVectorFromContacts(FloatArray &answer, TimeStep *tStep, CharType type, ValueModeType mode,
                                    const UnknownNumberingScheme &s, Domain *domain, FloatArray *eNorms = NULL);
    

    void assembleTangentFromContacts(SparseMtrx &answer, TimeStep *tStep,
                          CharType type, const UnknownNumberingScheme &r_s, const UnknownNumberingScheme &c_s);

};

} // end namespace oofem
#endif // contactmanager_h
