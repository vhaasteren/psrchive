/***************************************************************************
 *
 *   Copyright (C) 2024 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "LinearRegression.h"
#include "BoxMuller.h"
#include "random.h"
#include "Error.h"

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

static const unsigned ncoef = 2;  // scale and offset

double normalized_difference[ncoef] = { 0,0 };

bool subtract_weighted_mean_abscissa = false;
double covariance = 0.0;

static bool verbose = false;

void runtest () try
{
  double max_coeff = 3.0;
  double sigma = 0.1;

  double input_values [ncoef];

  for (unsigned i=0; i<ncoef; i++)
  {
    double value = 0;
    random_value (value, max_coeff);

    if (verbose)
      cerr << "c_" << i << "=" << value << endl;
    input_values[i] = value;
  }

  double in_scale = input_values[0];
  double in_offset = input_values[1];

  double x_min = -5.0;
  double x_max = 10.0;
  unsigned ndat = 60;
  double x_step = (x_max - x_min) / (ndat+1);

  std::vector<double> xval(ndat);  // x-ordinate of data
  std::vector<double> yval(ndat);  // y-ordinate of data
  std::vector<double> wt(ndat);  // weight of data

  BoxMuller gasdev;

  // ofstream os ("lm_test_covar.dat");

  for (unsigned i=0; i<ndat; i++)
  {
    double x = x_min + x_step * i;
    double y = in_offset + in_scale * x + sigma*gasdev();

    xval[i] = x;
    yval[i] = y;
    wt[i] = 1.0/(sigma*sigma);
  }

  LinearRegression fit;
  fit.subtract_weighted_mean_abscissa = subtract_weighted_mean_abscissa;
  fit.weighted_least_squares (yval, xval, wt);

  Estimate<double> scale = fit.scale;
  Estimate<double> offset = fit.offset;

  if (verbose)
    cerr << "scale=" << scale << " offset=" << offset << endl;

  double diff = input_values[0] - scale.val;
  normalized_difference[0] += diff * diff / scale.var;

  if (subtract_weighted_mean_abscissa)
    input_values[1] += scale.val * fit.weighted_mean_abscissa;

  diff = input_values[1] - offset.val;
  normalized_difference[1] += diff * diff / offset.var;

  covariance += fit.covariance;
}
catch (const Error& error) {
  cerr << error << endl;
}
catch (...) {
  cerr << "Unhandled exception" << endl;
}

unsigned ntest = 100000;
double tolerance = 0.02;

int run_multiple_tests()
{
  covariance = 0.0;
  normalized_difference[0] = normalized_difference[1] = 0;

  for (unsigned i=0; i<ntest; i++)
    runtest ();

  for (unsigned i=0; i<ncoef; i++)
  {
    double avg = normalized_difference[i]/ntest;
    cerr << i << " normalized error=" << avg << endl;

    if ( fabs(avg - 1.0) > tolerance)
    {
      cerr << "FAIL: uncertainty-normalized error is not close enough to 1" << endl;
      return -1;
    }
  }

  cerr << "covariance = " << covariance << endl;
  return 0;
}

int main ()
{
  cerr << "running " << ntest << " linear least-squares fits" << endl;
  
  if (run_multiple_tests() < 0)
    return -1;

  cerr << "running " << ntest << " linear least-squares fits with subtract_weighted_mean_abscissa = true" << endl;

  subtract_weighted_mean_abscissa = true;

  if (run_multiple_tests() < 0)
    return -1;

  // covariance expected to be zero
  if (covariance > 1e15)
  {
    cerr << "FAIL: covariance = " << covariance << " expected to be closer to zero" << endl;
    return -1;
  }

  cerr << "Test Passed! (all uncertainty-normalized errors are close to 1)" << endl;
  return 0;
}

