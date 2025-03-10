// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li


#include "sm/Contact/ActiveBc/node2nodepenaltycontact.h"
#include "utility/set.h"
#include "input/domain.h"
#include "dofman/node.h"
#include "math/floatmatrix.h"
#include "input/unknownnumberingscheme.h"
#include "math/sparsemtrx.h"
#include "engng/classfactory.h"

#ifdef _OPENMP
#include <omp.h>
#endif

namespace oofem {
REGISTER_BoundaryCondition(Node2NodePenaltyContact);


void
Node2NodePenaltyContact :: initializeFrom(InputRecord &ir)
{
    ActiveBoundaryCondition :: initializeFrom(ir);

    IR_GIVE_FIELD(ir, this->penalty, _IFT_Node2NodePenaltyContact_penalty);
    this->useTangent = ir.hasField(_IFT_Node2NodePenaltyContact_useTangent);


    IR_GIVE_FIELD(ir, this->masterSet, _IFT_Node2NodePenaltyContact_masterSet);
    IR_GIVE_FIELD(ir, this->slaveSet, _IFT_Node2NodePenaltyContact_slaveSet);
}


void
Node2NodePenaltyContact :: assemble(SparseMtrx &answer, TimeStep *tStep,
                                    CharType type, const UnknownNumberingScheme &r_s, 
                                    const UnknownNumberingScheme &c_s, double scale,
                                    void* lock)
{
    if ( !this->useTangent || type != TangentStiffnessMatrix ) {
        return;
    }


    FloatMatrix K;
    IntArray loc, c_loc;

    IntArray dofIdArray = {
        D_u, D_v
    };

    if ( masterSet.giveSize() != slaveSet.giveSize() ) {
        OOFEM_ERROR("Number of master nodes does not match number of slave nodes")
    }

    for ( int pos = 1; pos <= masterSet.giveSize(); ++pos ) {
        Node *masterNode = this->giveDomain()->giveNode(masterSet.at(pos) );
        Node *slaveNode = this->giveDomain()->giveNode(slaveSet.at(pos) );

        masterNode->giveLocationArray(dofIdArray, loc, r_s);
        slaveNode->giveLocationArray(dofIdArray, c_loc, c_s);
        loc.followedBy(c_loc);

        this->computeTangentFromContact(K, masterNode, slaveNode, tStep);
#ifdef _OPENMP
        if (lock) omp_set_lock(static_cast<omp_lock_t*>(lock));
#endif
        answer.assemble(loc, K);
#ifdef _OPENMP
        if (lock) omp_unset_lock(static_cast<omp_lock_t*>(lock));
#endif
    }
}

void
Node2NodePenaltyContact :: assembleVector(FloatArray &answer, TimeStep *tStep,
                                          CharType type, ValueModeType mode,
                                          const UnknownNumberingScheme &s, FloatArray *eNorms,
                                          void*lock)
{
    if ( type != ExternalForcesVector ) {
        return;
    }

    //IntArray dofIdArray = {D_u, D_v, D_w};
    IntArray dofIdArray = {
        D_u, D_v
    };

    IntArray loc, c_loc;
    FloatArray fext;

    if ( masterSet.giveSize() != slaveSet.giveSize() ) {
        OOFEM_ERROR("Number of master nodes does not match number of slave nodes")
    }

    for ( int pos = 1; pos <= masterSet.giveSize(); ++pos ) {
        Node *masterNode = this->giveDomain()->giveNode(masterSet.at(pos) );
        Node *slaveNode = this->giveDomain()->giveNode(slaveSet.at(pos) );

        masterNode->giveLocationArray(dofIdArray, loc, s);
        slaveNode->giveLocationArray(dofIdArray, c_loc, s);
        this->computeExternalForcesFromContact(fext, masterNode, slaveNode, tStep);
        loc.followedBy(c_loc);
#ifdef _OPENMP
        if (lock) omp_set_lock(static_cast<omp_lock_t*>(lock));
#endif
        answer.assemble(fext, loc);
#ifdef _OPENMP
        if (lock) omp_unset_lock(static_cast<omp_lock_t*>(lock));
#endif
    }
}






void
Node2NodePenaltyContact :: giveLocationArrays(std :: vector< IntArray > &rows, std :: vector< IntArray > &cols, CharType type, const UnknownNumberingScheme &r_s, const UnknownNumberingScheme &c_s)
{
    IntArray r_loc, c_loc;
    rows.resize(masterSet.giveSize() );
    cols.resize(masterSet.giveSize() );
    IntArray dofIdArray = {
        D_u, D_v
    };

    for ( int pos = 1; pos <= masterSet.giveSize(); ++pos ) {
        Node *masterNode = this->giveDomain()->giveNode(masterSet.at(pos) );
        Node *slaveNode = this->giveDomain()->giveNode(slaveSet.at(pos) );

        masterNode->giveLocationArray(dofIdArray, r_loc, r_s);
        slaveNode->giveLocationArray(dofIdArray, c_loc, c_s);

        // column block
        rows [ pos - 1 ] = r_loc;
        cols [ pos - 1 ] = c_loc;
    }
}









void
Node2NodePenaltyContact :: computeTangentFromContact(FloatMatrix &answer, Node *masterNode, Node *slaveNode, TimeStep *tStep)
{
    double gap;
    FloatArray Nv;
    this->computeGap(gap, masterNode, slaveNode, tStep);
    this->computeNormalMatrixAt(Nv, masterNode, slaveNode, tStep);
    answer.beDyadicProductOf(Nv, Nv);
    answer.times(this->penalty);
    if ( gap > 0.0 ) {
        answer.zero();
    }
}

void
Node2NodePenaltyContact :: computeGap(double &answer, Node *masterNode, Node *slaveNode, TimeStep *tStep)
{
    FloatArray uS, uM;
    auto xs = slaveNode->giveCoordinates();
    auto xm = masterNode->giveCoordinates();
    FloatArray normal = xs - xm;
    double norm = normal.computeNorm();
    if ( norm < 1.0e-8 ) {
        OOFEM_ERROR("Couldn't compute normal between master node (num %d) and slave node (num %d), nodes are too close to each other.",
                    masterNode->giveGlobalNumber(), slaveNode->giveGlobalNumber() );
    } else {
        normal.times(1.0 / norm);
    }

    slaveNode->giveUnknownVector(uS, { D_u, D_v }, VM_Total, tStep, true);
    masterNode->giveUnknownVector(uM, { D_u, D_v }, VM_Total, tStep, true);
    xs.add(uS);
    xm.add(uM);
    FloatArray dx = xs - xm;
    answer = dx.dotProduct(normal);
}


void
Node2NodePenaltyContact :: computeNormalMatrixAt(FloatArray &answer, Node *masterNode, Node *slaveNode, TimeStep *TimeStep)
{
    const auto &xs = slaveNode->giveCoordinates();
    const auto &xm = masterNode->giveCoordinates();
    auto normal = xs - xm;
    double norm = normal.computeNorm();
    if ( norm < 1.0e-8 ) {
        OOFEM_ERROR("Couldn't compute normal between master node (num %d) and slave node (num %d), nodes are too close to each other.",
                    masterNode->giveGlobalNumber(), slaveNode->giveGlobalNumber() );
    } else {
        normal.times(1.0 / norm);
    }
    // The normal is not updated for node2node which is for small deformations only
    // C = {n -n}
    answer = {
        normal.at(1), normal.at(2),
        -normal.at(1), -normal.at(2)
    };
}


void
Node2NodePenaltyContact :: computeExternalForcesFromContact(FloatArray &answer, Node *masterNode, Node *slaveNode, TimeStep *tStep)
{
    double gap;
    this->computeGap(gap, masterNode, slaveNode, tStep);
    this->computeNormalMatrixAt(answer, masterNode, slaveNode, tStep);
    if ( gap < 0.0 ) {
        answer.times(penalty * gap);
    } else {
        answer.times(0);
    }
}
} // namespace oofem
