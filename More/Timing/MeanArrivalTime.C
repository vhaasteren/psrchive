/***************************************************************************
 *
 *   Copyright (C) 2024 by Willem van Straten and Matthew Bailes
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/MeanArrivalTime.h"
#include "Pulsar/DispersionDelay.h"

#include "LinearRegression.h"
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

  LinearRegression fit;
  fit.subtract_weighted_mean_abscissa = true;
  fit.weighted_least_squares (yval, lambda_sq, wt);

  DispersionDelay delay;
  fit_delta_DM = delay.get_dispersion_measure(fit.scale);
  fit_delay = fit.offset;
  reference_frequency = Pulsar::speed_of_light / (sqrt(fit.weighted_mean_abscissa) * 1e6); // MHz
}
