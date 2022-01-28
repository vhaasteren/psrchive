//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

// psrchive/More/Polarimetry/Pulsar/SystemCalibratorManager.h

#ifndef __Pulsar_SystemCalibratorManager_H
#define __Pulsar_SystemCalibratorManager_H

#include "Pulsar/SystemCalibrator.h"

namespace Pulsar
{
  //! Manages multiple sources and a common model
  /*! Each SystemCalibrator manages input data from a single pulsar.
    This class manages input data from multiple pulsars using multiple
    SystemCalibrator instances that share a common SignalPath (for each
    frequency channel).
  */

  class SystemCalibratorManager : public Reference::Able
  {

  public:

    void manage (SystemCalibrator*);
    
    //! Return the reference epoch of the calibration experiment
    MJD get_epoch () const;

    //! Get the number of frequency channels
    unsigned get_nchan () const;

    //! Get the number of data points in the given frequency channel
    unsigned get_ndata (unsigned ichan) const;

    //! Get the total number of input polarization states (pulsar and cal)
    virtual unsigned get_nstate () const;
    
    //! Get the number of pulsar polarization states in the model
    virtual unsigned get_nstate_pulsar () const;

    //! Return true if the state index is a pulsar
    virtual unsigned get_state_is_pulsar (unsigned istate) const;

    //! Return true if calibrator (e.g. noise diode) data are incorporated
    virtual bool has_cal () const;

    //! Return a new plot information interface for the specified pulsar state
    virtual Calibrator::Info* new_info_pulsar (unsigned istate) const;

    //! Prepare the data for inclusion in the model
    virtual void preprocess (Archive* data);

    //! Add the observation to the set of constraints
    virtual void add_observation (const Archive* data);

    //! Get the epoch of the first observation
    MJD get_start_epoch () const;

    //! Get the epoch of the last observation
    MJD get_end_epoch () const;

    //! Pre-calibrate the polarization of the given archive
    virtual void precalibrate (Archive* archive);

  protected:

    //! The system calibrators
    std::vector< Reference::To<SystemCalibrator> > calibrator;

  };

}

#endif

