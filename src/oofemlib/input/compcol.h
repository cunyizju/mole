// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li


// Inspired by SPARSELib++
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*             ********   ***                                 SparseLib++    */
/*          *******  **  ***       ***      ***               v. 1.5c        */
/*           *****      ***     ******** ********                            */
/*            *****    ***     ******** ********              R. Pozo        */
/*       **  *******  ***   **   ***      ***                 K. Remington   */
/*        ********   ********                                 A. Lumsdaine   */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*                                                                           */
/*                                                                           */
/*                     SparseLib++ : Sparse Matrix Library                   */
/*                                                                           */
/*               National Institute of Standards and Technology              */
/*                        University of Notre Dame                           */
/*              Authors: R. Pozo, K. Remington, A. Lumsdaine                 */
/*                                                                           */
/*                                 NOTICE                                    */
/*                                                                           */
/* Permission to use, copy, modify, and distribute this software and         */
/* its documentation for any purpose and without fee is hereby granted       */
/* provided that the above notice appear in all copies and supporting        */
/* documentation.                                                            */
/*                                                                           */
/* Neither the Institutions (National Institute of Standards and Technology, */
/* University of Notre Dame) nor the Authors make any representations about  */
/* the suitability of this software for any purpose.  This software is       */
/* provided ``as is'' without expressed or implied warranty.                 */
/*                                                                           */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*        Compressed column sparse matrix  (O-based, Fortran)            */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#ifndef compcol_h
#define compcol_h

#include "math/sparsemtrx.h"
#include "math/intarray.h"

#define _IFT_CompCol_Name "csc"

namespace oofem {
/**
 * Implementation of sparse matrix stored in compressed column storage.
 */
class OOFEM_EXPORT CompCol : public SparseMtrx
{
protected:

    FloatArray val; // data values (nz_ elements)
    IntArray rowind;    // row_ind (nz_ elements)
    IntArray colptr;    // col_ptr (dim_[1]+1 elements)

    int base;              // index base: offset of first element
    int nz;                // number of nonzeros

public:
    /** Constructor. Before any operation an internal profile must be built.
     * @see buildInternalStructure
     */
    CompCol(int n=0);
    /// Copy constructor
    CompCol(const CompCol & S);
    /// Assignment operator
    CompCol &operator = ( const CompCol & C );
    /// Destructor
    virtual ~CompCol() { }

    // Overloaded methods:
    std::unique_ptr<SparseMtrx> clone() const override;
    void times(const FloatArray &x, FloatArray &answer) const override;
    void timesT(const FloatArray &x, FloatArray &answer) const override;
    void times(double x) override;
    int buildInternalStructure(EngngModel *, int, const UnknownNumberingScheme &s) override;
    int assemble(const IntArray &loc, const FloatMatrix &mat) override;
    int assemble(const IntArray &rloc, const IntArray &cloc, const FloatMatrix &mat) override;
    bool canBeFactorized() const override { return false; }
    void zero() override;
    double &at(int i, int j) override;
    double at(int i, int j) const override;
    void toFloatMatrix(FloatMatrix &answer) const override;
    void printYourself() const override;
    const char* giveClassName() const override { return "CompCol"; }
    SparseMtrxType giveType() const override { return SMT_CompCol; }
    bool isAsymmetric() const override { return true; }

    // Breaks encapsulation, but access is needed for PARDISO and SuperLU solvers;
    FloatArray &giveValues() { return val; }
    IntArray &giveRowIndex() { return rowind; }
    IntArray &giveColPtr() { return colptr; }

    // Methods needed by SuperLU interface
    const int giveNumberOfNonzeros() {return nz;}

    const double &values(int i) const { return val[i]; }
    const int &row_ind(int i) const { return rowind[i]; }
    const int &col_ptr(int i) const { return colptr[i]; }

protected:
    /***********************************/
    /*  General access function (slow) */
    /***********************************/
    /// implements 0-based access
    double operator() (int i, int j) const;
    /// implements 0-based access
    double &operator() (int i, int j);
};
} // end namespace oofem
#endif // compcol_h
