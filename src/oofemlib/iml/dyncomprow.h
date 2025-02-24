// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*        Compressed row sparse matrix  (O-based, Fortran)               */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#ifndef dyncomprow_h
#define dyncomprow_h

#include "math/sparsemtrx.h"
#include "math/intarray.h"

#define _IFT_DynCompRow_Name "dcsr"

namespace oofem {
// alloc chunk for columns
#define DynCompRow_CHUNK 8


/**
 * Implementation of sparse matrix stored in compressed column storage.
 * Designed to allow simple dynamic runtime grow of receiver.
 */
class OOFEM_EXPORT DynCompRow : public SparseMtrx
{
protected:
    /// data values per column
    std::vector<FloatArray> rows;
    /// row_ind per column
    std::vector<IntArray> colind;
    /// pointers to the diagonal elements; needed only for ILU
    IntArray diag;

    /// index base: offset of first element
    int base;

public:
    /** Constructor. Before any operation an internal profile must be built.
     * @see builInternalStructure
     */
    DynCompRow(int n=0);
    /// Copy constructor
    DynCompRow(const DynCompRow & S);
    /// Assignment operator
    DynCompRow &operator = ( const DynCompRow & C );
    /// Destructor
    virtual ~DynCompRow() {}

    std::unique_ptr<SparseMtrx> clone() const override;
    void times(const FloatArray &x, FloatArray &answer) const override;
    void timesT(const FloatArray &x, FloatArray &answer) const override;
    void times(double x) override;
    int buildInternalStructure(EngngModel *, int, const UnknownNumberingScheme &) override;
    int assemble(const IntArray &loc, const FloatMatrix &mat) override;
    int assemble(const IntArray &rloc, const IntArray &cloc, const FloatMatrix &mat) override;
    bool canBeFactorized() const override { return false; }
    void zero() override;
    const char* giveClassName() const override { return "DynCompRow"; }
    SparseMtrxType  giveType() const override { return SMT_DynCompRow; }
    bool isAsymmetric() const override { return true; }
    void printStatistics() const override;
    double &at(int i, int j) override;
    double at(int i, int j) const override;

    /**
     * Performs LU factorization on yourself; modifies receiver
     * This routine computes the L and U factors of the ILU(p).
     */
    void ILUPYourself(int part_fill = 5, double drop_tol = 1.e-8);
    void ILUPsolve(const FloatArray &x, FloatArray &y) const;
    void ILUPtrans_solve(const FloatArray &x, FloatArray &y) const;

    /*******************************/
    /*  Access and info functions  */
    /*******************************/
    /// Returns col index for i-th  row
    const IntArray &col_ind(int i) const { return colind[ i ]; }
    /// Returns row values
    const FloatArray &row(int i) const { return rows[ i ]; }

protected:

    /***********************************/
    /*  General access function (slow) */
    /***********************************/
    /// implements 0-based access
    double operator() (int i, int j) const;
    /// implements 0-based access
    double &operator() (int i, int j);

    /// returns the column index of given column at given row, else returns zero.
    int giveColIndx(int row, int col) const;
    /// insert column entry into row, preserving order of column indexes, returns the index of new row.
    int insertColInRow(int row, int col);

    void checkSizeTowards(IntArray &);
    void checkSizeTowards(const IntArray &rloc, const IntArray &cloc);
    void growTo(int);

    void qsortRow(IntArray &ind, IntArray &ir, FloatArray &val, int l, int r);
    int qsortRowPartition(IntArray &ind, IntArray &ir, FloatArray &val, int l, int r);
};
} // end namespace oofem
#endif // dyncomprow_h
