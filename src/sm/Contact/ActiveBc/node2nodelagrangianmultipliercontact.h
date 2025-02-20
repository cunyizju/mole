// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef node2nodelagrangianmultipliercontact_h
#define node2nodelagrangianmultipliercontact_h


#include "bc/activebc.h"


///@name Input fields for _IFT_ContactElement
//@{
#define _IFT_Node2NodeLagrangianMultiplierContact_Name "n2nlagrangianmultipliercontact"
#define _IFT_Node2NodeLagrangianMultiplierContact_useTangent "usetangent"

#define _IFT_Node2NodeLagrangianMultiplierContact_masterSet "masterset"
#define _IFT_Node2NodeLagrangianMultiplierContact_slaveSet "slaveset"




//@}

namespace oofem {
class Domain;
class SparseMtrx;
class TimeStep;
class DofManager;
class GaussPoint;
class UnknownNumberingScheme;
class FloatMatrix;
class IntegrationRule;
class ContactElement;
class Node;

class OOFEM_EXPORT Node2NodeLagrangianMultiplierContact : public ActiveBoundaryCondition
{
protected:
    bool useTangent; ///< Determines if tangent should be used.
    IntArray slaveSet;
    IntArray masterSet;
    std :: vector< std :: shared_ptr< DofManager > >lmdm;
public:

    /// Constructor.
    Node2NodeLagrangianMultiplierContact(int n, Domain *d);
    //: ActiveBoundaryCondition(n, d) { }
    /// Destructor.
    virtual ~Node2NodeLagrangianMultiplierContact() {};

    void initializeFrom(InputRecord &ir) override;

    void assemble(SparseMtrx &answer, TimeStep *tStep,
                  CharType type, const UnknownNumberingScheme &r_s, const UnknownNumberingScheme &c_s, double scale = 1.0,
                  void*lock=nullptr) override;

    void assembleVector(FloatArray &answer, TimeStep *tStep,
                        CharType type, ValueModeType mode,
                        const UnknownNumberingScheme &s, FloatArray *eNorms = NULL,
                        void *lock=nullptr) override;


    const char *giveClassName() const override { return "Node2NodeLagrangianMultiplierContact"; }
    const char *giveInputRecordName() const override { return _IFT_Node2NodeLagrangianMultiplierContact_Name; }

    int giveNumberOfInternalDofManagers() override { return masterSet.giveSize(); }
    DofManager *giveInternalDofManager(int i) override { return this->lmdm.at(i - 1).get(); }



    double computeTangentFromContact(FloatMatrix &answer, Node *masterNode, Node *slaveNode, TimeStep *tStep);
    void computeGap(double &answer,  Node *masterNode, Node *slaveNode, TimeStep *tStep);

    void computeNormalMatrixAt(FloatArray &answer,  Node *masterNode, Node *slaveNode, TimeStep *TimeStep);


    void computeExternalForcesFromContact(FloatArray &answer,  Node *masterNode, Node *slaveNode, TimeStep *tStep);

    void giveLocationArrays(std :: vector< IntArray > &rows, std :: vector< IntArray > &cols, CharType type, const UnknownNumberingScheme &r_s, const UnknownNumberingScheme &c_s) override;
    void giveLagrangianMultiplierLocationArray(const UnknownNumberingScheme &r_s, std :: vector< IntArray > &answer);
};
} // end namespace oofem
#endif // node2nodecontact_h
