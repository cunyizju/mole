// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li


#include "math/gjacobi.h"
#include "math/mathfem.h"
#include "math/floatmatrix.h"

namespace oofem {
GJacobi :: GJacobi(Domain *d, EngngModel *m) :
    NumericalMethod(d, m)
{
    nsmax  = 15;     // default maximum number of sweeps allowed
    rtol   = 10.E-12; // convergence tolerance
}

GJacobi :: ~GJacobi() { }


#define GJacobi_ZERO_CHECK_TOL 1.e-40

ConvergedReason
GJacobi :: solve(FloatMatrix &a, FloatMatrix &b, FloatArray &eigv, FloatMatrix &x)
//
// this function solve the generalized eigenproblem using the Generalized
// jacobi iteration
//
//
{
    int nsweep, nr;
    double eps, eptola, eptolb, akk, ajj, ab, check, sqch, d1, d2, den, ca, cg, ak, bk, xj, xk;
    double aj, bj;
    int jm1, kp1, km1, jp1;

    if ( a.giveNumberOfRows() != b.giveNumberOfRows() ||
        !a.isSquare() || !b.isSquare() ) {
        OOFEM_ERROR("A matrix, B mtrix -> size mismatch");
    }

    int n = a.giveNumberOfRows();
    eigv.resize(n);
    x.resize(n, n);

    //
    // Create temporary arrays
    //
    FloatArray d(n);
    //
    // Initialize EigenValue and EigenVector Matrices
    //
    for ( int i = 1; i <= n; i++ ) {
        //      if((a.at(i,i) <= 0. ) && (b.at(i,i) <= 0.))
        //        OOFEM_ERROR("Matrices are not positive definite");
        d.at(i) = a.at(i, i) / b.at(i, i);
        eigv.at(i) = d.at(i);
    }

    x.beUnitMatrix();

    if ( n == 1 ) {
        return CR_CONVERGED;
    }

    //
    // Initialize sweep counter and begin iteration
    //
    nsweep = 0;
    nr = n - 1;

    do {
        nsweep++;
# ifdef DETAILED_REPORT
        OOFEM_LOG_DEBUG("*GJacobi*: sweep number %4d\n", nsweep);
#endif
        //
        // check if present off-diagonal element is large enough to require zeroing
        //
        eps = pow(0.01, ( double ) nsweep);
        eps *= eps;
        for ( int j = 1; j <= nr; j++ ) {
            int jj = j + 1;
            for ( int k = jj; k <= n; k++ ) {
                eptola = ( a.at(j, k) * a.at(j, k) ) / ( a.at(j, j) * a.at(k, k) );
                eptolb = ( b.at(j, k) * b.at(j, k) ) / ( b.at(j, j) * b.at(k, k) );
                if ( ( eptola  < eps ) && ( eptolb < eps ) ) {
                    continue;
                }

                //
                // if zeroing is required, calculate the rotation matrix elements ca and cg
                //
                akk = a.at(k, k) * b.at(j, k) - b.at(k, k) * a.at(j, k);
                ajj = a.at(j, j) * b.at(j, k) - b.at(j, j) * a.at(j, k);
                ab  = a.at(j, j) * b.at(k, k) - a.at(k, k) * b.at(j, j);
                check = ( ab * ab + 4.0 * akk * ajj ) / 4.0;
                if ( fabs(check) < GJacobi_ZERO_CHECK_TOL ) {
                    check = fabs(check);
                } else if ( check < 0.0 ) {
                    OOFEM_ERROR("Matrices are not positive definite");
                }

                sqch = sqrt(check);
                d1 = ab / 2. + sqch;
                d2 = ab / 2. - sqch;
                den = d1;
                if ( fabs(d2) > fabs(d1) ) {
                    den = d2;
                }

                if ( den != 0.0 ) {                  // strange !
                    ca = akk / den;
                    cg = -ajj / den;
                } else {
                    ca = 0.0;
                    cg = -a.at(j, k) / a.at(k, k);
                }

                //
                // perform the generalized rotation to zero
                //
                if ( ( n - 2 ) != 0 ) {
                    jp1 = j + 1;
                    jm1 = j - 1;
                    kp1 = k + 1;
                    km1 = k - 1;
                    if ( ( jm1 - 1 ) >= 0 ) {
                        for ( int i = 1; i <= jm1; i++ ) {
                            aj = a.at(i, j);
                            bj = b.at(i, j);
                            ak = a.at(i, k);
                            bk = b.at(i, k);
                            a.at(i, j) = aj + cg * ak;
                            b.at(i, j) = bj + cg * bk;
                            a.at(i, k) = ak + ca * aj;
                            b.at(i, k) = bk + ca * bj;
                        }
                    }

                    if ( ( kp1 - n ) <= 0 ) {
                        for ( int i = kp1; i <= n; i++ ) { // label 140
                            aj = a.at(j, i);
                            bj = b.at(j, i);
                            ak = a.at(k, i);
                            bk = b.at(k, i);
                            a.at(j, i) = aj + cg * ak;
                            b.at(j, i) = bj + cg * bk;
                            a.at(k, i) = ak + ca * aj;
                            b.at(k, i) = bk + ca * bj;
                        }
                    }

                    if ( ( jp1 - km1 ) <= 0 ) { // label 160
                        for ( int i = jp1; i <= km1; i++ ) {
                            aj = a.at(j, i);
                            bj = b.at(j, i);
                            ak = a.at(i, k);
                            bk = b.at(i, k);
                            a.at(j, i) = aj + cg * ak;
                            b.at(j, i) = bj + cg * bk;
                            a.at(i, k) = ak + ca * aj;
                            b.at(i, k) = bk + ca * bj;
                        }
                    }
                }                           // label 190

                ak = a.at(k, k);
                bk = b.at(k, k);
                a.at(k, k) = ak + 2.0 *ca *a.at(j, k) + ca *ca *a.at(j, j);
                b.at(k, k) = bk + 2.0 *ca *b.at(j, k) + ca *ca *b.at(j, j);
                a.at(j, j) = a.at(j, j) + 2.0 *cg *a.at(j, k) + cg * cg * ak;
                b.at(j, j) = b.at(j, j) + 2.0 *cg *b.at(j, k) + cg * cg * bk;
                a.at(j, k) = 0.0;
                b.at(j, k) = 0.0;
                //
                // update the eigenvector matrix after each rotation
                //
                for ( int i = 1; i <= n; i++ ) {
                    xj = x.at(i, j);
                    xk = x.at(i, k);
                    x.at(i, j) = xj + cg * xk;
                    x.at(i, k) = xk + ca * xj;
                }                        // label 200
            }
        }                                // label 210

        //
        // update the eigenvalues after each sweep
        //
#ifdef DETAILED_REPORT
        OOFEM_LOG_DEBUG("GJacobi: a,b after sweep\n");
        a.printYourself();
        b.printYourself();
#endif
        for ( int i = 1; i <= n; i++ ) {
            // in original uncommented
            //      if ((a.at(i,i) <= 0.) || (b.at(i,i) <= 0.))
            //        error ("solveYourselfAt: Matrices are not positive definite");
            eigv.at(i) = a.at(i, i) / b.at(i, i);
        }                                          // label 220

# ifdef DETAILED_REPORT
        OOFEM_LOG_DEBUG("GJacobi: current eigenvalues are:\n");
        eigv.printYourself();
        OOFEM_LOG_DEBUG("GJacobi: current eigenvectors are:\n");
        x.printYourself();
# endif
        //
        // check for convergence
        //
        for ( int i = 1; i <= n; i++ ) {       // label 230
            double tol = rtol * d.at(i);
            double dif = ( eigv.at(i) - d.at(i) );
            if ( fabs(dif) > tol ) {
                goto label280;
            }
        }                                 // label 240

        //
        // check all off-diagonal elements to see if another sweep is required
        //
        eps = rtol * rtol;
        for ( int j = 1; j <= nr; j++ ) {
            int jj = j + 1;
            for ( int k = jj; k <= n; k++ ) {
                double epsa = ( a.at(j, k) * a.at(j, k) ) / ( a.at(j, j) * a.at(k, k) );
                double epsb = ( b.at(j, k) * b.at(j, k) ) / ( b.at(j, j) * b.at(k, k) );
                if ( ( epsa < eps ) && ( epsb < eps ) ) {
                    continue;
                }

                goto label280;
            }
        }                                 // label 250

        //
        // fill out bottom triangle of resultant matrices and scale eigenvectors
        //
        break;
        // goto label255 ;
        //
        // update d matrix and start new sweep, if allowed
        //
label280:
        d = eigv;
    } while ( nsweep < nsmax );

    // label255:
    for ( int i = 1; i <= n; i++ ) {
        for ( int j = 1; j <= n; j++ ) {
            a.at(j, i) = a.at(i, j);
            b.at(j, i) = b.at(i, j);
        }                               // label 260
    }

    for ( int j = 1; j <= n; j++ ) {
        double bb = sqrt( fabs( b.at(j, j) ) );
        for ( int k = 1; k <= n; k++ ) {
            x.at(k, j) /= bb;
        }
    }                                  // label 270

    if (nsweep<nsmax) {
      return CR_CONVERGED;
    } else {
      return CR_DIVERGED_ITS;
    }
}
} // end namespace oofem
