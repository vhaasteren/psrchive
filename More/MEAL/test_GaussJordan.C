/***************************************************************************
 *
 *   Copyright (C) 2022 - 2024 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "MEAL/GaussJordan.h"
#include "Jacobi.h"
#include "random.h"
#include <vector>

using namespace std;

const unsigned dim = 73;

#define PRINT_MATRIX 0

int once ()
{
  vector< vector<double> > test (dim, vector<double>(dim));
  random_matrix (test, 10.0);

  vector< vector<double> > copy = test;

  vector< vector<double> > other (dim, vector<double>(1) );
  random_matrix (other, 10.0);

  double log_abs_det_A = MEAL::GaussJordan (test, other);

#if PRINT_MATRIX
  cerr << "log(abs(det(A)))=" << log_abs_det_A << endl;
#endif

  double tolerance = 2e-12;

  Matrix<dim,dim,double> matrix;

  for (unsigned irow=0; irow < dim; irow++)
  {
    for (unsigned icol=0; icol < dim; icol++)
    {
      matrix[irow][icol] = copy[irow][icol];

#if PRINT_MATRIX
      cerr << copy[irow][icol] << "\t";
#endif

      double sum = 0.0;
      for (unsigned k=0; k < dim; k++)
        sum += test[irow][k] * copy[k][icol];

      double expect = (irow == icol) ? 1.0 : 0.0;
      if ( fabs(sum - expect) > tolerance)
      {
        cerr << "irow=" << irow << " icol=" << icol << " sum=" << sum << endl;
        return -1;
      }
    }

#if PRINT_MATRIX
    cerr << endl;
#endif
  }

#if JACOBI_IS_FIXED

  // estimate the determinant via the product of eigenvalues
  Matrix<dim,dim,double> evec;
  Vector<dim,double> eval;
  Jacobi (matrix, evec, eval);

  double det_Jacobi = 1.0;
  for (unsigned irow=0; irow < dim; irow++)
    det_Jacobi *= eval[irow];

  if (fabs(det - det_Jacobi) > tolerance)
  {
    cerr << "determinant (via GaussJordan) = " << det << endl
         << "determinant (via Jacobi) = " << det_Jacobi << endl;
    return -1;
  }

#endif

  return 0;
}

int main ()
{
  unsigned ntrial = 1000;
  unsigned nerr = 0;

  cerr << "performing Gauss-Jordan elimination on " << ntrial << " " << dim << "x" << dim << " matrices" << endl;
  for (unsigned i=0; i < ntrial; i++) try
  {
    if (once () != 0)
      nerr ++;
  }
  catch (Error& error)
  {
    cerr << error << endl;
    nerr ++;
  }

  cerr << nerr << " errors in " << ntrial << " trials" << endl;
  return 0;
}

