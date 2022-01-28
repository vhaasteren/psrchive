/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/SystemCalibratorManager.h"

using namespace Pulsar;

void SystemCalibratorManager::manage (SystemCalibrator* cal)
{
  if (calibrator.size() > 0)
    cal->share (calibrator[0]);

  /* TO DO: think about how to manage different channels flagged as
     invalid for different pulsars */
  
  calibrator.push_back (cal);
}

//! Return the reference epoch of the calibration experiment
MJD SystemCalibratorManager::get_epoch () const
{
  return 0.0;
}

//! Get the number of data points in the given frequency channel
unsigned SystemCalibratorManager::get_ndata (unsigned ichan) const
{
  return 0;
}

//! Get the total number of input polarization states (pulsar and cal)
unsigned SystemCalibratorManager::get_nstate () const
{
  return 0;
}

//! Get the number of pulsar polarization states in the model
unsigned SystemCalibratorManager::get_nstate_pulsar () const
{
  return 0;
}

//! Return true if calibrator (e.g. noise diode) data are incorporated
bool SystemCalibratorManager::has_cal () const
{
  return false;
}

//! Return a new plot information interface for the specified pulsar state
Calibrator::Info*
SystemCalibratorManager::new_info_pulsar (unsigned istate) const
{
  return 0;
}

//! Prepare the data for inclusion in the model
void SystemCalibratorManager::preprocess (Archive* data)
{
}

//! Add the observation to the set of constraints
void SystemCalibratorManager::add_observation (const Archive* data)
{
}

//! Get the epoch of the first observation
MJD SystemCalibratorManager::get_start_epoch () const
{
  return 0.0;
}

//! Get the epoch of the last observation
MJD SystemCalibratorManager::get_end_epoch () const
{
  return 0.0;
}

//! Pre-calibrate the polarization of the given archive
void SystemCalibratorManager::precalibrate (Archive* archive)
{
}
