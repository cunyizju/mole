// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#include "quasicontinuumnumberingscheme.h"
#include <algorithm>

namespace oofem {
QuasicontinuumNumberingscheme :: QuasicontinuumNumberingscheme() :
    UnknownNumberingScheme()
    , neq(0)
    , pres_neq(0)
    , isInitialized(false)
{ }

void
QuasicontinuumNumberingscheme :: init(Domain *domain, std :: vector< bool >activatedNodeList, TimeStep *tStep)
{
    isInitialized = true;
    /*
     * // compute list of selected nodes numbers
     * int noActovedNodes = std::count_if(activatedNodeList.begin(), activatedNodeList.end(), [](bool i){return i;});
     * this->selectedNodes.resize(noActovedNodes);
     * selectedNodes.zero();
     * int k = 0;
     * for (int i=1; i<=(int)activatedNodeList.size(); i++) {
     *  if (activatedNodeList[i-1]) {
     *      k++;
     *      selectedNodes.at(k) = i;
     *  }
     * }
     */
    this->selectedNodes.resize( activatedNodeList.size() );
    selectedNodes.zero();
    for ( int i = 0; i < ( int ) activatedNodeList.size(); i++ ) {
        if ( activatedNodeList [ i ] ) {
            selectedNodes[i] = 1;
        }
    }


    //int nnode = domain->giveNumberOfDofManagers();
    //int nnode = 0;
    //   this->dofEquationNumbers.resize(nnode);

    equationMap.clear();

    for ( int inode = 1; inode <= selectedNodes.giveSize(); inode++ ) {
        if ( selectedNodes.at(inode) == 1 ) {
            std :: map< int, int > dof2EquationMap;
            auto idofman = domain->giveDofManager(inode);
            IntArray dofIDArray;
            idofman->giveCompleteMasterDofIDArray(dofIDArray);
            if ( !idofman->hasAnySlaveDofs() ) {
                for ( auto &dofid : dofIDArray ) {
                    if ( idofman->giveDofWithID( dofid )->hasBc(tStep) ) {
                        dof2EquationMap [ dofid ] = --pres_neq;
                    } else {
                        dof2EquationMap [ dofid ] = ++neq;
                    }
                }
            } else {
#if 0
                IntArray masterDofIDArray, masterDofMans;
                //idofman->giveMasterDofIDArray({D_u, D_v},masterDofIDArray);
                idofman->giveMasterDofMans(masterDofMans);
                for (int m = 1; m <= masterDofMans.giveSize(); m++ ) {
                    auto mdofman = domain->giveDofManager(masterDofMans.at(m));
                    mdofman->giveCompleteMasterDofIDArray(dofIDArray);
                    for ( auto &dofid : dofIDArray ) {
                        if(mdofman->giveDofWithID(dofid)->hasBc(tStep)) {
                            dof2EquationMap [ dofid ] = --pres_neq;
                        } else {
                            dof2EquationMap [ dofid ] = ++neq;
                        }
                    }
                 }
#endif
            }

            this->equationMap.insert( {inode, std::move(dof2EquationMap)} );
        }
    }
}

void
QuasicontinuumNumberingscheme :: reset()
{
    neq = 0;
    pres_neq = 0;
}

int
QuasicontinuumNumberingscheme :: giveDofEquationNumber(Dof *dof) const
{
    int dofEqNum = 0;

    if ( selectedNodes.at( dof->giveDofManNumber() ) == 1 ) {
        dofEqNum = ( equationMap.find( dof->giveDofManNumber() )->second ).find( dof->giveDofID() )->second;
    } else {
        dofEqNum = 0;
    }

    //this->nodalEquationNumbers.at( dof->giveDofManNumber() );
    if ( dofEqNum < 0 ) {
        dofEqNum = 0;
    }

    return dofEqNum;
}


int
QuasicontinuumNumberingscheme :: giveTotalNumberOfEquations() const
{
    return neq;
}


int
QuasicontinuumNumberingscheme :: giveRequiredNumberOfDomainEquation() const
{
    return neq;
}

int
QuasicontinuumNumberingscheme :: giveTotalNumberOfPrescribedEquations() const
{
    return -1 * pres_neq;
}
} // end namespace oofem
