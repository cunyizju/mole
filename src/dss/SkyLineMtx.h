// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef _SKYLINEMTX_H__
#define _SKYLINEMTX_H__

#include "SparseMatrixF.h"
#include "Ordering.h"
#include "BigMatrix.h"

DSS_NAMESPASE_BEGIN

/**
 * @author Richard Vondracek
 */

class SkyLineMtx :
    public TraceableMatrix,
    public ILargeMatrix
{
public:
    SkyLineMtx(SparseMatrixF &sm, Ordering *order, MathTracer *eMT);
    virtual ~SkyLineMtx();

public:
    int *column_starts;
    double *columndata;
    double *D;
    Ordering *order;

protected:
    long n;
    long nonzeros;

    void GrowSkyline(int i, int j, long *column_ns);
    void AllocateMemory(IConectMatrix *spm, int neq);

public:
    long N() const { return n; }
    long Nonzeros() const { return ( long ) columns_data_length; }

    // This data is used in the Sealed state
    long columns_data_length;

    //ILargeMatrix
    virtual void WriteStatistics(long no_init_blocks, long no_nonzeros);
    virtual long No_Multiplications() { return 0; }

    virtual void Solve(double *b, double *x) = 0;

    //ILargeMatrix
    //virtual void LoadZeros();
    //virtual void LoadMatrixNumbers(SparseMatrixF& sm) = 0;
    //virtual void SolveLV(const LargeVector& b, LargeVector& x) = 0;
    virtual void Factorize() = 0;
    //virtual void MultiplyByVector(const LargeVectorAttach& x, LargeVectorAttach& y) = 0;

public:
    virtual void SchurComplementFactorization(int fixed_blocks) = 0;
    virtual void SolveA11(double *x, long fixed_blocks) = 0;
    virtual void Sub_A21_A11inv(double *x, long fixed_blocks) = 0;
    virtual void Sub_A11inv_A12(double *x, long fixed_blocks) = 0;
    virtual void WriteCondensedMatrixA22(double *a, Ordering *mcn, IntArrayList *lncn) = 0;
}; //class SkyLineMtx

DSS_NAMESPASE_END

#endif // _SKYLINEMTX_H__
