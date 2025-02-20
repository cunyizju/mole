// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef quasicontinuumnumberingscheme_h
#define quasicontinuumnumberingscheme_h

#include "input/unknownnumberingscheme.h"
#include "dofman/dof.h"
#include "input/domain.h"
#include "dofman/dofmanager.h"

namespace oofem {
/**
 * Numbering scheme that takes into account only list of selected nodes
 *
 * @author Martin Horak
 */
class QuasicontinuumNumberingscheme : public UnknownNumberingScheme
{
protected:
    Domain *domain;
    /// Container storing particular equation numbers for each node
    IntArray nodalEquationNumbers;
    /// Selected nodes
    IntArray selectedNodes;
    /// Last given number of equation
    int neq;
    /// Last given number of prescribed equation
    int pres_neq;
    /// Flag controlling wether the numbering has been initialized or not
    bool isInitialized;
    /// map form dofid to equation number
    std::map<int, std::map<int,int>> equationMap;


public:
    /// Constructor
    QuasicontinuumNumberingscheme();
    /// Destructor
    virtual ~QuasicontinuumNumberingscheme() {}

    /**
     * Initializes the receiver
     */
    void init(Domain *domain, std::vector<bool> activatedNodeList, TimeStep *tStep);
    bool isDefault() const override { return true; }
    int giveDofEquationNumber(Dof *dof) const override;
    int giveRequiredNumberOfDomainEquation() const override;

    /// Returns total number of equations
    virtual int giveTotalNumberOfEquations() const;

    /// Returns total number of prescribed equations
    virtual int giveTotalNumberOfPrescribedEquations() const;

    /// Resets the numbering in order to start numbering again from 1
    virtual void reset();
	bool giveIsInitializedFlag() {return isInitialized;}
};

} // end namespace oofem
#endif // quasicontinuumnumberingscheme_h
