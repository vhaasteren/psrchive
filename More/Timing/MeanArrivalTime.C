/***************************************************************************
 *
 *   Copyright (C) 2024 by Willem van Straten and Matthew Bailes
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/MeanArrivalTime.h"
#include "Pulsar/DispersionDelay.h"

#include "Physical.h"

using namespace std;
using namespace Pulsar;

void MeanArrivalTime::integrate (double freq_MHz, Estimate<double> delay_seconds)
{
  double lambda = Pulsar::speed_of_light / (freq_MHz * 1e6); // metres

  delay_sec.push_back (delay_seconds);
  lambda_sq.push_back (lambda*lambda);
}

//! Delete all data and reset all values to zero
void MeanArrivalTime::reset ()
{
  delay_sec.clear();
  lambda_sq.clear();

  fit_delay = 0.0;
  fit_delta_DM = 0.0;
  reference_frequency = 0.0;
}

void MeanArrivalTime::fit ()
{
  unsigned ndat = delay_sec.size();
  
  vector<double> yval (ndat);
  vector<double> wt (ndat);

  for (unsigned idat=0; idat < ndat; idat++)
  {
    wt[idat] = 1.0 / delay_sec[idat].var;
    yval[idat] = delay_sec[idat].val;
  }

  linear_fit.subtract_weighted_mean_abscissa = true;
  linear_fit.weighted_least_squares (yval, lambda_sq, wt);

  DispersionDelay delay;
  fit_delta_DM = delay.get_dispersion_measure(linear_fit.scale);
  fit_delay = linear_fit.offset;
  reference_frequency = Pulsar::speed_of_light / (sqrt(linear_fit.weighted_mean_abscissa) * 1e6); // MHz
}

vector<pair<Estimate<double>,double>> MeanArrivalTime::get_subband_delays()
{
  vector<pair<Estimate<double>,double>> result(num_subbands);

  unsigned nchan = delay_sec.size();
  unsigned nchan_per_subband = nchan / num_subbands;

  for (unsigned isub=0; isub < num_subbands; isub++)
  {
    /*
    Compute the inverse-variance weighted mean squared wavelength for the current subband.
    Each squared wavelength is weighted by the inverse variance of the shift estimate.
    */
    MeanEstimate<double> weighted_mean_lambda_sq;
    for (unsigned ichan=0; ichan < nchan_per_subband; ichan++)
    {
      unsigned jchan = isub * nchan_per_subband + ichan;
      Estimate<double> weighted_lambda_sq (lambda_sq[jchan], delay_sec[jchan].var);
      weighted_mean_lambda_sq += weighted_lambda_sq;
    }

    double weighted_lambda_sq = weighted_mean_lambda_sq.get_Estimate().get_value();
    double delta_lambda_sq = weighted_lambda_sq - linear_fit.weighted_mean_abscissa;
    Estimate<double> shift = linear_fit.offset + linear_fit.scale * delta_lambda_sq;
    double frequency = Pulsar::speed_of_light / (sqrt(weighted_lambda_sq) * 1e6); // MHz

    result[isub].first = shift;
    result[isub].second = frequency;
  }

  return result;
}
