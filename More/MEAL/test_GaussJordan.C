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

int main ()
{
  vector< vector<double> > test (13, vector<double>(13) );
  random_matrix (test, 10.0);

  vector< vector<double> > identity (13, vector<double>(13) );
  for (unsigned i=0; i<13; i++)
    for (unsigned j=0; j<13; j++)
      if (i==j)
        identity[i][j] = 1.0;
      else
        identity[i][j] = 0.0;


  MEAL::GaussJordan (test, identity);

  for (unsigned i=0; i<13; i++)
  {
    for (unsigned j=0; j<13; j++)
      cerr << test[i][j] << " ";
    cerr << endl;
  }

  cerr << endl << endl;

  for (unsigned i=0; i<13; i++)
  {
    for (unsigned j=0; j<13; j++)
      cerr << identity[i][j] << " ";
    cerr << endl;
  }

  return 0;
}

