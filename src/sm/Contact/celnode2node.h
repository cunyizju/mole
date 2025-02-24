// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef celnode2node_h
#define celnode2node_h

#include "contact/contactelement.h"

///@name Input fields for _IFT_ContactElement
//@{
//#define _IFT_ContactManager_Name "contactmanager"

//@}

namespace oofem {
class Domain;
class ContactManager;
class ContactDefinition;
class SparseMtrx;
class TimeStep;
class DofManager;
class GaussPoint;
class UnknownNumberingScheme;
class FloatMatrix;
class IntegrationRule;
class ContactElement;

class OOFEM_EXPORT Node2NodeContact : public ContactElement
{
protected:
    ContactDefinition *cDef;

private:
    DofManager *masterNode;
    DofManager *slaveNode;

    // should be set by input:
    double area; // The area associated with the node (default = 1)- in order to represent some physical dimension.  
    double epsN;  // penalty stiffness 

    FloatArray normal;
public:

    /// Constructor.
    Node2NodeContact(DofManager *master, DofManager *slave);
    /// Destructor.
    virtual ~Node2NodeContact(){};
    int instanciateYourself(DataReader &dr) override;
    void setupIntegrationPoints() override;

    virtual void computeGap(FloatArray &answer, TimeStep *tStep);
    virtual void computeContactTractionAt(GaussPoint *gp, FloatArray &t, FloatArray &gap, TimeStep *tStep);
    virtual void computeCmatrixAt(GaussPoint *gp, FloatArray &answer, TimeStep *TimeStep);
    FloatArray &giveNormal() { return this->normal; }


    // Necessary methods - pure virtual in base class
    void computeContactForces(FloatArray &answer, TimeStep *tStep, ValueModeType mode,
                              const UnknownNumberingScheme &s, Domain *domain, FloatArray *eNorms) override;

    void computeContactTangent(FloatMatrix &answer, TimeStep *tStep) override;

    void giveLocationArray(IntArray &answer, const UnknownNumberingScheme &s) override;
};


class OOFEM_EXPORT Node2NodeContactL : public Node2NodeContact
{
protected:
    ContactDefinition *cDef;

private:
    DofManager *masterNode;
    DofManager *slaveNode;
    int lagrangeId; // dof Id associated with the Lagrange multiplier

    // should be set by input:
    double area; // The area associated with the node (default = 1)- in order to represent some physical dimension.  

public:
    /// Constructor.
    Node2NodeContactL(DofManager *master, DofManager *slave);
    /// Destructor.
    virtual ~Node2NodeContactL(){};
    void giveDofManagersToAppendTo(IntArray &answer) override; 
    void computeContactTractionAt(GaussPoint *gp, FloatArray &t, FloatArray &gap, TimeStep *tStep) override;

    // Necessary methods - pure virtual in base class
    void computeContactForces(FloatArray &answer, TimeStep *tStep, ValueModeType mode,
                              const UnknownNumberingScheme &s, Domain *domain, FloatArray *eNorms) override;

    void computeContactTangent(FloatMatrix &answer, TimeStep *tStep) override;

    void giveLocationArray(IntArray &answer, const UnknownNumberingScheme &s) override;
};

} // end namespace oofem
#endif // celnode2node_h
