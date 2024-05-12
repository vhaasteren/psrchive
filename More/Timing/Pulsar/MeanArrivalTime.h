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

    //! Reference frequency
    double reference_frequency = 0;

  public:

    //! Add a time delay estimate
    void integrate (double freq_MHz, Estimate<double> delay_seconds);

    //! Compute the best-fit delay and delta-DM
    void fit ();

    //! Return the best-fit delay (in seconds) at the reference frequency
    Estimate<double> get_delay () const { return fit_delay; }

    //! Return the best-fit delta-DM (in \f$ {\rm pc\,cm}^{-3} \f$) at the reference frequency
    Estimate<double> get_delta_DM () const { return fit_delta_DM; }

    //! Return the reference frequency in MHz
    /*! At this frequency, the covariance between delay and delta-DM is zero */
    double get_reference_frequency () const { return reference_frequency; }
  };

}

#endif
