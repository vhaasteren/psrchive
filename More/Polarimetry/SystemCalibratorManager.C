/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/SystemCalibratorManager.h"
#include "Pulsar/Archive.h"

using namespace Pulsar;
using namespace std;

SystemCalibratorManager::SystemCalibratorManager ()
{
  fscrunch_data_to_model = false;
}

void SystemCalibratorManager::manage (SystemCalibrator* cal)
{
  if (calibrator.size() > 0)
    cal->share (calibrator[0]);

  /* TO DO: think about how to manage different channels flagged as
     invalid for different pulsars */
  
  calibrator.push_back (cal);
}

//! Return the number of managed system calibrators
unsigned SystemCalibratorManager::get_ncalibrator ()
{
  return calibrator.size ();
}

//! Return the ith system calibrator
SystemCalibrator* SystemCalibratorManager::get_calibrator (unsigned i)
{
  return calibrator[i];
}

//! Return the fiducial system calibrator
SystemCalibrator* SystemCalibratorManager::get_model ()
{
  // TO DO: define fiducial calibrator
  return calibrator[0];
}

SystemCalibrator* SystemCalibratorManager::get_calibrator (const Archive* data)
{
  bool throw_exception = false;

  // TO DO: match will fail if model nchan != data nchan
  for (auto cal: calibrator)
  {
    if (fscrunch_data_to_model)
      cal->match_check_nchan = false;
	
    bool match = cal->match (data, throw_exception);

    cal->match_check_nchan = true;

    if (match)
      return cal;
  }
  
  throw Error (InvalidState, "SystemCalibratorManager::get_calibrator",
	       "no model matches filename=" + data->get_filename()); 
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
  SystemCalibrator* model = get_calibrator (data);
  
  if (fscrunch_data_to_model &&
      model->get_nchan() < data->get_nchan())
  {
    cerr << "SystemCalibratorManager::preprocess frequency integrating data"
      " (nchan=" << data->get_nchan() << ") to match model"
      " (nchan=" << model->get_nchan() << ")" << endl;
    data->fscrunch_to_nchan (model->get_nchan());
  }

  model->preprocess (data);
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
