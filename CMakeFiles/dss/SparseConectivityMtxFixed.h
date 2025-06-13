// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

using System;
using System.Collections;
using System.Diagnostics;
using System.Runtime.InteropServices;

using Golem;

namespace MathKer
{
/**
 * This is a fixed sparse matrix format
 * There is full set of rows but in each row are stored only nonzeros
 *
 * @author Richard Vondracek
 */
public class SparseConectivityMtxFixed : IConectMatrix
{
public:
    IntArrayList **ColumnsIndexes;
    int n;
    int nonzeros;

public:
    int N()                 { return n; }
    int Nonzeros()  { return nonzeros; }


    public void Init(int N)
    {
        n = N;
        ColumnsIndexes = new IntArrayList * [ n ];
    }

    ~SparseConectivityMtxFixed()
    {
        for ( int i = 0; i < n; i++ ) {
            if ( ColumnsIndexes [ i ] ) { delete ColumnsIndexes [ i ];
                                          ColumnsIndexes [ i ] = NULL; } }

        delete [] ColumnsIndexes;
    }

    IntArrayList *GetIndexesAboveDiagonalInColumn(int j)
    {
        return ColumnsIndexes [ j ];
    }
}
}
