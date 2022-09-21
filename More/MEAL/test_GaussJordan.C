/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "MEAL/GaussJordan.h"
#include "random.h"
#include <vector>

using namespace std;

int once ()
{
  unsigned dim = 273;

  vector< vector<double> > test (dim, vector<double>(dim) );
  random_matrix (test, 10.0);

  vector< vector<double> > copy = test;

  vector< vector<double> > other (dim, vector<double>(1) );
  random_matrix (other, 10.0);

  MEAL::GaussJordan (test, other);

  double tolerance = 2e-12;

  for (unsigned irow=0; irow < dim; irow++)
    for (unsigned icol=0; icol < dim; icol++)
    {
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

  return 0;
}

int main ()
{
  unsigned ntrial = 1000;
  unsigned nerr = 0;

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

