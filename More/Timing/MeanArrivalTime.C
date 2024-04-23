/***************************************************************************
 *
 *   Copyright (C) 2024 by Willem van Straten and Matthew Bailes
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/MeanArrivalTime.h"
#include "ChiSquared.h"
#include "Physical.h"

using namespace std;
using namespace Pulsar;

void MeanArrivalTime::integrate (double freq_MHz, Estimate<double> delay_seconds)
{
  double lambda = Pulsar::speed_of_light / (freq_MHz * 1e6); // metres

  delay_sec.push_back (delay_seconds);
  lambda_sq.push_back (lambda*lambda);
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

  weighted_linear_fit (fit_delta_DM, fit_delay, yval, lambda_sq, wt);
}
