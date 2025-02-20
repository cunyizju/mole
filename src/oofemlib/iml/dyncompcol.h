// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*        Compressed column sparse matrix  (O-based, Fortran)            */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#ifndef dyncompcol_h
#define dyncompcol_h

#include "math/sparsemtrx.h"
#include "math/intarray.h"

#define _IFT_DynCompCol_Name "dcsc"

namespace oofem {
// alloc chunk for columns
#define DynCompCol_CHUNK 8
// turns on the stl set container, used to store column data, usually slower

/**
 * Implementation of sparse matrix stored in compressed column storage.
 * Designed to allow simple dynamic runtime grow of receiver.
 */
class OOFEM_EXPORT DynCompCol : public SparseMtrx
{
protected:
    std::vector<FloatArray> columns; // data values per column
    std::vector<IntArray> rowind;   // row_ind per column

    int base; // index base: offset of first element

public:
    /** Constructor. Before any operation an internal profile must be built.
     * @see builInternalStructure
     */
    DynCompCol(int n=0);
    /// Copy constructor
    DynCompCol(const DynCompCol & S);
    /// Assignment operator
    DynCompCol &operator = ( const DynCompCol & C );
    /// Destructor
    virtual ~DynCompCol() {}

    // Overloaded methods:
    std::unique_ptr<SparseMtrx> clone() const override;
    void times(const FloatArray &x, FloatArray &answer) const override;
    void timesT(const FloatArray &x, FloatArray &answer) const override;
    void times(double x) override;
    int buildInternalStructure(EngngModel *, int, const UnknownNumberingScheme &) override;
    int assemble(const IntArray &loc, const FloatMatrix &mat) override;
    int assemble(const IntArray &rloc, const IntArray &cloc, const FloatMatrix &mat) override;
    bool canBeFactorized() const override { return false; }
    void zero() override;
    const char* giveClassName() const override { return "DynCompCol"; }
    SparseMtrxType giveType() const override { return SMT_DynCompCol; }
    bool isAsymmetric() const override { return true; }
    double &at(int i, int j) override;
    double at(int i, int j) const override;
    void printStatistics() const override;

    /*******************************/
    /*  Access and info functions  */
    /*******************************/
    /// Returns row index for i-th  column
    const IntArray &row_ind(int i) const { return rowind[ i ]; }
    /// Returns column values
    const FloatArray &column(int i) const { return columns[ i ]; }

protected:

    /***********************************/
    /*  General access function (slow) */
    /***********************************/
    /// Implements 0-based access
    double operator() (int i, int j) const;
    /// Implements 0-based access
    double &operator() (int i, int j);

    /// Returns the row index of given row at given column, else returns zero.
    int giveRowIndx(int col, int row) const;
    /// Insert row entry into column, preserving order of row indexes, returns the index of new row.
    int insertRowInColumn(int col, int row);

    void checkSizeTowards(IntArray &);
    void checkSizeTowards(const IntArray &rloc, const IntArray &cloc);
    void growTo(int);
};
} // end namespace oofem
#endif // dyncompcol_h
