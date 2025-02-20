// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef contactdefinition_h
#define contactdefinition_h

#include "oofemcfg.h"
#include "input/datareader.h"
#include "input/inputrecord.h"
#include "contact/contactmanager.h"

///@name Input fields for _IFT_ContactDefinition
//@{
#define _IFT_ContactDefinition_Name "contactdefinition"
//@}

namespace oofem {
class Domain;
class ContactManager;
class ContactObject;
class ContactElement;
class TimeStep;

class ContactMaterial; // write this


/**
 * This class manages a particular contact definition. 
 * This keeps track of the discretization, how the contact constraints are enforced 
 *
 * @author Jim Brouzoulis
 */
class OOFEM_EXPORT ContactDefinition
{
private:
    ContactManager *cMan;

    std :: vector< ContactElement *> masterElementList;
    int numberOfConstraintEq; // used when creating new dofs
    
    //ContactMaterial *contactMaterial;
    
public:
    /// Constructor.
    ContactDefinition(ContactManager *cMan);
    /// Destructor.
    virtual ~ContactDefinition();

    ContactDefinition(const ContactDefinition& src) = delete;
    ContactDefinition &operator = (const ContactDefinition &src) = delete;

    virtual void createContactDofs();

    virtual void initializeFrom(InputRecord &ir) { }

    virtual int instanciateYourself(DataReader &dr);
    virtual const char *giveClassName() const { return "ContactDefinition"; }
    //virtual const char *giveInputRecordName() const { return _IFT_ContactDefinition_Name; }
    ContactManager *giveContactManager() { return this->cMan; }
    virtual int giveNumberOfConstraintEqToAdd() { return this->numberOfConstraintEq; }
    virtual void setNumberOfConstraintEqToAdd(const int number) { this->numberOfConstraintEq = number; }
    virtual void computeContactForces(FloatArray &answer, TimeStep *tStep, ValueModeType mode,
                                const UnknownNumberingScheme &s, Domain *domain, FloatArray *eNorms);
    
    virtual void computeContactTangent(SparseMtrx &answer, TimeStep *tStep,
                      const UnknownNumberingScheme &r_s, const UnknownNumberingScheme &c_s); 
    
    ContactElement *giveContactElement(const int num) { return this->masterElementList[num-1]; }
    int giveNumbertOfContactElements() { return this->masterElementList.size(); }
    void addContactElement(ContactElement *cEl) { this->masterElementList.push_back(cEl); }
    // objects can be of different kinds
    // nodes, segments, surfaces, analytical functions
    
    /* Each contact definition has several master objects that each keep track of generally several slave objects
     * Ex:
     * -Master can be a node and only keep track of another node -> node2node
     * -Master can be a node and keep track of several other nodes -> for larger displacements
     * -Master can be a segment and only keep track of another segment -> segment2segment 
     * -etc.
     *
     * how should they be stored? 
     */
    //MasterObjects
    //SlaveObjects
    // assembleVectorOf...
    // assembleTangentOf...
    
    
    
    //ConstrainType - How should the contact constraints be fulfilled
    // Penalty, Lagrange multiplier, Augmented (mix), Mortar (weakly fulfilled)
    
    // InterfaceModel - a constitutive model for the contact - depends on the physics
    // linear/nonlinear for stresses, thermal conductance, etc.
    // Normal/tangential
};


} // end namespace oofem
#endif // contactdefinition_h
