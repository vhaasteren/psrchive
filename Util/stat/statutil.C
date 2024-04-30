/***************************************************************************
 *
 *   Copyright (C) 2024 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "statutil.h"
#include "FTransform.h"

#include <algorithm>
#include <numeric>

#include <cassert>
#include <cmath>

using namespace std;

// worker function for variance, skewness, kurtosis, etc.
void central_moments (vector<double> data, vector<double>& mu)
{
  if (mu.size() == 0)
    return;

  vector<double> prod (data.size(), 1.0);

  for (unsigned i=0; i < mu.size(); i++)
  {
    double total = 0.0;

    for (unsigned j=0; j < data.size(); j++)
    {
      prod[j] *= data[j];
      total += prod[j];
    }

    mu[i] = total / data.size();

    if (i == 0)
    {
      // subract the mean so that further moments are central
      for (unsigned j=0; j < data.size(); j++)
      {
        prod[j] -= mu[0];
        data[j] = prod[j];
      }
    }
  }
}

void power_spectral_density (const vector<double>& data, vector<float>& fps)
{
  vector<float> copy (data.begin(), data.end());
  fps.resize (data.size() + 2);
  
  FTransform::frc1d (data.size(), &fps[0], &copy[0]);

  const unsigned nbin = data.size() / 2 + 1;
  for (unsigned ibin=0; ibin < nbin; ibin++)
  {
    float re = fps[ibin*2];
    float im = fps[ibin*2+1];
    fps[ibin] = re*re + im*im;
  }

  fps.resize (nbin);
  
  if (FTransform::get_norm() == FTransform::unnormalized)
    for (auto& element : fps)
      element /= data.size();
}

double median_upper_harmonic (const vector<double>& data, vector<float>* fps)
{
  vector<float> tmp;
  if (fps == 0)
    fps = &tmp;
  
  power_spectral_density (data, *fps);
  vector<double> upper_half (fps->begin() + fps->size()/2, fps->end());

  double med = median (upper_half);
  // cerr << "median_upper_harmonic = " << med << endl;
  return med;
}

double robust_variance (const vector<double>& data, vector<float>* fps)
{
  // divide by log(2) because spectral power has exponential distribution
  return median_upper_harmonic (data, fps) / log(2.0);
}
