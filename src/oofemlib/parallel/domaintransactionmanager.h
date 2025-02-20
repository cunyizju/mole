// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef domaintransactionmanager_h
#define domaintransactionmanager_h

#include "oofemcfg.h"

#include <map>

namespace oofem {
class FEMComponent;
class Domain;
class DofManager;
class Element;
/**
 * Class representing domain transaction manager. The purpose of this class is to
 * make the domain modification in terms of adding and deleting components) possible.
 *
 * The changes are recorded in transaction manager and until the are committed,
 * no change is reflected in domain itself.
 *
 * When adding a new component has to be created and send as transaction parameter.
 * The allocated component will be further maintained (and deallocated) in transaction manager or domain itself.
 * When adding component a reference to already existing as well as newly added (but not yet committed)
 * component must be possible. Therefore, the concept of temporary numbering is introduced.
 * Existing domain components are assigned with temporary numbers equal to to their local numbers
 * (in parallel version to their global numbers) and newly added components can be assigned with
 * arbitrary temporary number (but which should be different from those already used). Then references
 * between components for newly added components is specified using temporary numbering.
 * After transactions are committed, the local numbering can change.
 *
 * The user is responsible for data consistency, so for example if a node is removed,
 * but an element exist that needs such node, then (hopefully)
 * an error will be thrown since there is no mechanism for enforcing data consistency.
 *
 * Transaction manager provides an access to recorded transactions; so it is possible
 * to modify an existing transaction instead of posting a new one.
 * However, transaction manager does not provides access to original unmodified domain components,
 * since this can lead to their modification and since in the current implementation the transaction
 * manager keeps only modified records, the original records are not returned. They must be requested from original domain,
 * so it is clear that their modification is not part of transaction but it directly changes domain data.
 */
class OOFEM_EXPORT DomainTransactionManager
{
public:
    enum DomainComponentType { DCT_DofManager, DCT_Element };
    enum DomainTransactionType { DTT_ADD, DTT_Remove };
    /*
     * struct DTM_Transaction {
     * /// transaction type
     * DomainTransactionType _ttype;
     * /// component type
     * DomainComponentType _ctype;
     * /// temporary number
     * int _num;
     * /// component adress
     * FemComponent* _obj;
     * };
     */
protected:
    Domain *domain;
    std :: map< int, DofManager * >dofmanTransactions;
    std :: map< int, Element * >elementTransactions;
    //list<DTM_Transaction> transactions;

public:
    DomainTransactionManager(Domain * d);
    ~DomainTransactionManager();

    void initialize();
    int addDofManTransaction(DomainTransactionType, int, DofManager *);
    int addElementTransaction(DomainTransactionType, int, Element *);
    DofManager *giveDofManager(int label);
    Element *giveElement(int label);
    int  commitTransactions();


    friend class Domain;
};
} // end namespace oofem

#endif // domaintransactionmanager_h
