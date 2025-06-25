/***************************************************************************
 *
 *   Copyright (C) 2025 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/*
 * This program numerically demonstrates that the product
 * of two orthogonal boosts is equivalent to the product of a boost in the 
 * plane defined by the two boost axes and a rotation about an axis
 * perpendicular to that plane.
 */

#include "MEAL/Rotation.h"
#include "Pauli.h"

using namespace std;

int main (int argc, char** argv) try
{
  MEAL::Rotation permutation( {1, 1, 1}, -M_PI/3 );

  Jones<double> R = permutation.evaluate();

  cerr << "R=" << R << endl;

  return 0;

} catch (Error& error) {
  cerr << error << endl;
  return -1;
}

