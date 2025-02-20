// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef eigenvectorprimaryfield_h
#define eigenvectorprimaryfield_h

#include "fields/dofdistributedprimaryfield.h"

namespace oofem {
/**
 * Class representing the mode shapes of eigen vectors.
 * The values are stored as VM_Total only.
 * Active vector is determined by the time step number.
 */
class OOFEM_EXPORT EigenVectorPrimaryField : public DofDistributedPrimaryField
{
public:
    /**
     * Constructor. Creates a field of given type associated to given domain.
     * @param a Engineering model which field belongs to.
     * @param idomain Index of domain for field.
     * @param ft Type of stored field.
     * @param nHist Number of old time steps to store (minimum 1), i.e. the number of eigen vectors.
     */
    EigenVectorPrimaryField(EngngModel * a, int idomain, FieldType ft, int nHist);
    virtual ~EigenVectorPrimaryField();

    double giveUnknownValue(Dof *dof, ValueModeType mode, TimeStep *tStep) override;

    /**
     * Stores all the eigenvectors in one call.
     * @param eigenVectors Matrix with all eigen vectors (stored as columns)
     * @param s Equation numbering for the rows of the vectors.
     */
    void updateAll(const FloatMatrix &eigenVectors, const UnknownNumberingScheme &s);

    void applyDefaultInitialCondition() override;
    void advanceSolution(TimeStep *tStep) override;
};
} // end namespace oofem
#endif // eigenvectorprimaryfield_h
