// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef errorcheckingexportmodule_h_
#define errorcheckingexportmodule_h_

#include <vector>
#include <memory>
#include <fstream>

#include "export/exportmodule.h"
#include "input/valuemodetype.h"
#include "input/internalstatetype.h"

///@name Input fields for ErrorCheckingExportModule
//@{
#define _IFT_ErrorCheckingExportModule_Name "errorcheck"
#define _IFT_ErrorCheckingExportModule_filename "filename" ///< Filename where rules are defined (normally the input file).
#define _IFT_ErrorCheckingExportModule_writeIST "writeist" ///< Which internal state types to write rules for.
#define _IFT_ErrorCheckingExportModule_extractormode "extract" /// optional pram triggering extractor mode (output in every time step ignoring tstep specified)
//@}

namespace oofem {
class Domain;
class Element;
class DofManager;


/**
 * Error checking rule used for regressions tests.
 * @author Mikael Öhman
 */
class OOFEM_EXPORT ErrorCheckingRule
{
protected:
    int tstep = 0;
    int tsubstep = 0;
    int number = 0;
    double tolerance = 0.;
    double value = 0.; // expected value
    double computedValue = 0.;

public:
    ErrorCheckingRule(double tol) : tolerance(tol) { }
    virtual ~ErrorCheckingRule() = default;

    /** Checks if the rule is correct**/
    virtual bool check(Domain *domain, TimeStep *tStep) = 0;
    // returns the computed value (in given solution step) 
    virtual bool getValue(double& value, Domain* domain, TimeStep *tStep)=0;

    bool checkValue(double computedValue);
    virtual const char *giveClassName() const = 0;
};

/// Checks a node value
class OOFEM_EXPORT NodeErrorCheckingRule : public ErrorCheckingRule
{
protected:
    int dofid = 0;
    ValueModeType mode = VM_Unknown;

public:
    NodeErrorCheckingRule(const std :: string &line, double tol);
    bool check(Domain *domain, TimeStep *tStep) override;
    bool getValue(double& value, Domain* domain, TimeStep *tStep) override;
    const char *giveClassName() const override { return "NodeErrorCheckingRule"; }
};

/// Checks an element value
class OOFEM_EXPORT ElementErrorCheckingRule : public ErrorCheckingRule
{
protected:
    int irule = 0;
    int gpnum = 0;
    InternalStateType ist = IST_Undefined;
    int component = 0;

public:
    ElementErrorCheckingRule(const std :: string &line, double tol);
    bool check(Domain *domain, TimeStep *tStep) override;
    bool getValue(double& value, Domain* domain, TimeStep *tStep) override;
    const char *giveClassName() const override { return "ElementErrorCheckingRule"; }
};

/// Checks an internal element dofman value
class OOFEM_EXPORT InternalElementDofManErrorCheckingRule : public ErrorCheckingRule
{
protected:
    int idofman = 0;
    int dofid = 0;
    ValueModeType mode = VM_Unknown;

public:
    InternalElementDofManErrorCheckingRule(const std :: string &line, double tol);
    bool check(Domain *domain, TimeStep *tStep) override;
    bool getValue(double& value, Domain* domain, TimeStep *tStep) override;
    const char *giveClassName() const override { return "InternalElementDofManErrorCheckingRule"; }
};

/// Checks a beam element value (in terms of  end forces and and-displacements)
class OOFEM_EXPORT BeamElementErrorCheckingRule : public ErrorCheckingRule
{
public:
    enum BeamElementValueType{
        BET_localEndDisplacement,
        BET_localEndForces
    };

protected:
    BeamElementValueType ist = BET_localEndDisplacement;
    int component = 0;

public:
    BeamElementErrorCheckingRule(const std :: string &line, double tol);
    bool check(Domain *domain, TimeStep *tStep) override;
    bool getValue(double& value, Domain* domain, TimeStep *tStep) override; 
    const char *giveClassName() const override { return "BeamElementErrorCheckingRule"; }
};


/// Checks a reaction force value
class OOFEM_EXPORT ReactionErrorCheckingRule : public ErrorCheckingRule
{
protected:
    int dofid = 0;

public:
    ReactionErrorCheckingRule(const std :: string &line, double tol);
    bool check(Domain *domain, TimeStep *tStep) override;
    bool getValue(double& value, Domain* domain, TimeStep *tStep) override;
    const char *giveClassName() const override { return "ReactionErrorCheckingRule"; }
};

/// Checks a reaction force value
class OOFEM_EXPORT LoadLevelErrorCheckingRule : public ErrorCheckingRule
{
public:
    LoadLevelErrorCheckingRule(const std :: string &line, double tol);
    bool check(Domain *domain, TimeStep *tStep) override;
    bool getValue(double& value, Domain* domain, TimeStep *tStep) override;
    const char *giveClassName() const override { return "LoadLevelErrorCheckingRule"; }
};

/// Checks eigen value
class OOFEM_EXPORT EigenValueErrorCheckingRule : public ErrorCheckingRule
{
public:
    EigenValueErrorCheckingRule(const std :: string &line, double tol);
    bool check(Domain *domain, TimeStep *tStep) override;
    bool getValue(double& value, Domain* domain, TimeStep *tStep) override;
    const char *giveClassName() const override { return "EigenValueErrorCheckingRule"; }
};

/// Checks a reaction force value
class OOFEM_EXPORT TimeCheckingRule : public ErrorCheckingRule
{
public:
    TimeCheckingRule(const std :: string &line, double tol);
    bool check(Domain *domain, TimeStep *tStep) override;
    bool getValue(double& value, Domain* domain, TimeStep *tStep) override;
    const char *giveClassName() const override { return "TimeCheckingRule"; }
};



/**
 * Checks error in analysis (for automatic regression tests).
 * Exits with error if results are incorrect.
 *
 * @author Mikael Öhman (and others)
 */
class OOFEM_EXPORT ErrorCheckingExportModule : public ExportModule
{
protected:
    std :: string filename;
    std :: vector< std :: unique_ptr< ErrorCheckingRule > > errorCheckingRules;
    bool allPassed = true;
    bool writeChecks = false;
    IntArray writeIST;
    bool extractorMode = false;
    FILE* outputFile; // for extractorMode

    bool scanToErrorChecks(std :: ifstream &stream, double &errorTolerance);
    std::unique_ptr<ErrorCheckingRule> giveErrorCheck(std :: ifstream &stream, double errorTolerance);

    void writeCheck(Domain *domain, TimeStep *tStep);

public:
    ErrorCheckingExportModule(int n, EngngModel * e);
    ErrorCheckingExportModule(const ErrorCheckingExportModule &) = delete;
    ErrorCheckingExportModule &operator=(const ErrorCheckingExportModule &) = delete;

    void initialize() override;
    void terminate() override;
    void initializeFrom(InputRecord &ir) override;
    void doOutput(TimeStep *tStep, bool forcedOutput = false) override;

    const char *giveClassName() const override { return "ErrorCheckingExportModule"; }
    const char *giveInputRecordName() const { return _IFT_ErrorCheckingExportModule_Name; }
};
} // end namespace oofem
#endif // errorcheckingexportmodule_h_
