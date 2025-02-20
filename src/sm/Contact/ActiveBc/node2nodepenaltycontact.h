// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef node2nodecontact_h
#define node2nodecontact_h


#include "bc/activebc.h"


///@name Input fields for _IFT_ContactElement
//@{
#define _IFT_Node2NodePenaltyContact_Name "n2npenaltycontact"
#define _IFT_Node2NodePenaltyContact_penalty "penalty"
#define _IFT_Node2NodePenaltyContact_useTangent "usetangent"

#define _IFT_Node2NodePenaltyContact_masterSet "masterset"
#define _IFT_Node2NodePenaltyContact_slaveSet "slaveset"




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

class OOFEM_EXPORT Node2NodePenaltyContact : public ActiveBoundaryCondition
{
protected:
    bool useTangent; ///< Determines if tangent should be used.
    double penalty;
    IntArray slaveSet;
    IntArray masterSet;
public:

    /// Constructor.
    Node2NodePenaltyContact(int n, Domain *d) : ActiveBoundaryCondition(n, d) { }
    /// Destructor.
    virtual ~Node2NodePenaltyContact() {};

    void initializeFrom(InputRecord &ir) override;

    void assemble(SparseMtrx &answer, TimeStep *tStep,
                          CharType type, const UnknownNumberingScheme &r_s, const UnknownNumberingScheme &c_s, 
                          double scale = 1.0, void* lock=nullptr) override;

    void assembleVector(FloatArray &answer, TimeStep *tStep,
                        CharType type, ValueModeType mode,
                        const UnknownNumberingScheme &s, FloatArray *eNorms = NULL, void*lock=nullptr) override;


    const char *giveClassName() const override { return "Node2NodePenaltyContact"; }
    const char *giveInputRecordName() const override { return _IFT_Node2NodePenaltyContact_Name; }


    void computeTangentFromContact(FloatMatrix &answer, Node *masterNode, Node *slaveNode, TimeStep *tStep);
    void computeGap(double &answer,  Node *masterNode, Node *slaveNode, TimeStep *tStep);

    void computeNormalMatrixAt(FloatArray &answer,  Node *masterNode, Node *slaveNode, TimeStep *TimeStep);


    void computeExternalForcesFromContact(FloatArray &answer,  Node *masterNode, Node *slaveNode, TimeStep *tStep);

    void giveLocationArrays(std :: vector< IntArray > &rows, std :: vector< IntArray > &cols, CharType type, const UnknownNumberingScheme &r_s, const UnknownNumberingScheme &c_s) override;

};
} // end namespace oofem
#endif // node2nodecontact_h
