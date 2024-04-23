//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2024 by Willem van Straten and Matthew Bailes
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

// psrchive/More/Timing/Pulsar/MeanArrivalTime.h

#ifndef __Pulsar_MeanArrivalTime_h
#define __Pulsar_MeanArrivalTime_h

#include "ReferenceAble.h"
#include "Estimate.h"
#include <vector>

namespace Pulsar {

  //! Computes inverse-variance weighted mean frequency and arrival time, with optional delta-DM
  class MeanArrivalTime : public Reference::Able
  {
    //! time delay
    std::vector< Estimate<double> > delay_sec;
    //! wavelength squared
    std::vector< double > lambda_sq;

    //! Best-fit phase delay at reference frequency
    Estimate<double> fit_delay;
    //! Best-fit delta-DM at reference frequency
    Estimate<double> fit_delta_DM;   

    void fit ();
    
  public:

    //! Add a time delay estimate
    void integrate (double freq_MHz, Estimate<double> delay_seconds);

  };

}

#endif
