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


#ifndef symcompcol_h
#define symcompcol_h

#include "input/compcol.h"

namespace oofem {
/**
 * Implementation of symmetric sparse matrix stored using compressed column/row storage.
 * Only the lower part is stored.
 */
class OOFEM_EXPORT SymCompCol : public CompCol
{
public:
    /**
     * Constructor.
     * Before any operation an internal profile must be built.
     * @param n Size of matrix
     * @see buildInternalStructure
     */
    SymCompCol(int n=0);
    /// Copy constructor
    SymCompCol(const SymCompCol & S);
    /// Destructor
    virtual ~SymCompCol() { }

    std::unique_ptr<SparseMtrx> clone() const override;
    void times(const FloatArray &x, FloatArray &answer) const override;
    void timesT(const FloatArray &x, FloatArray &answer) const override { this->times(x, answer); }
    void times(double x) override;
    int buildInternalStructure(EngngModel *, int, const UnknownNumberingScheme &) override;
    int assemble(const IntArray &loc, const FloatMatrix &mat) override;
    int assemble(const IntArray &rloc, const IntArray &cloc, const FloatMatrix &mat) override;
    bool canBeFactorized() const override { return false; }
    void zero() override;
    double &at(int i, int j) override;
    double at(int i, int j) const override;
    const char* giveClassName() const override { return "SymCompCol"; }
    SparseMtrxType giveType() const  override { return SMT_SymCompCol; }
    bool isAsymmetric() const override { return false; }

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
#endif // symcompcol_h
