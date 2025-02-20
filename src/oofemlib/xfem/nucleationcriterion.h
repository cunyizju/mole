// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef SRC_OOFEMLIB_XFEM_NUCLEATIONCRITERION_H_
#define SRC_OOFEMLIB_XFEM_NUCLEATIONCRITERION_H_


#include <memory>
#include <vector>

namespace oofem {

class EnrichmentItem;
class Domain;
class DataReader;
class DynamicDataReader;
class InputRecord;
class EnrichmentFunction;

class NucleationCriterion
{
public:
    NucleationCriterion(Domain *ipDomain);
    virtual ~NucleationCriterion();

    virtual std::vector<std::unique_ptr<EnrichmentItem>> nucleateEnrichmentItems();

    virtual void initializeFrom(InputRecord &ir);
    virtual int instanciateYourself(DataReader &dr);
    virtual void postInitialize() {}

    virtual void appendInputRecords(DynamicDataReader &oDR);

    /// @return Class name of the receiver.
    virtual const char *giveClassName() const = 0;
    /// @return Input record name of the receiver.
    virtual const char *giveInputRecordName() const = 0;

protected:
    Domain *mpDomain;
    std::unique_ptr<EnrichmentFunction> mpEnrichmentFunc;
};

} /* namespace oofem */

#endif /* SRC_OOFEMLIB_XFEM_NUCLEATIONCRITERION_H_ */
