/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/SystemCalibratorManager.h"
#include "Pulsar/Archive.h"

#include <cassert>

using namespace Pulsar;
using namespace std;

SystemCalibratorManager::SystemCalibratorManager ()
{
}

void SystemCalibratorManager::manage (SystemCalibrator* cal)
{
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
    else if (Archive::verbose > 1)
      cerr << "SystemCalibratorManager::get_calibrator mismatch reason=" << cal->get_mismatch_reason() << endl;
  }
  
  throw Error (InvalidState, "SystemCalibratorManager::get_calibrator",
	       "no model matches filename=" + data->get_filename()); 
}

void SystemCalibratorManager::solve ()
{
  assert (calibrator.size() > 0);
  
  for (auto cal: calibrator)
  {
    if (!cal->has_calibrator())
      throw Error (InvalidState, "SystemCalibratorManager::solve",
		   "SystemCalibrator for " + cal->get_name() + " has no data");
    
    if (Archive::verbose > 1)
      cerr << "SystemCalibratorManager::solve preparing name="
          << cal->get_calibrator()->get_source () << endl;
    
    cal->solve_prepare ();
  }

  if (Archive::verbose > 1)
    cerr << "SystemCalibratorManager::solve calling SystemCalibrator::solve" << endl;
  
  get_model()->solve ();
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

//! Return true if the state index is a pulsar
unsigned SystemCalibratorManager::get_state_is_pulsar (unsigned istate) const
{
  return false;
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
  if (Archive::verbose > 1)
  {
    cerr << "SystemCalibratorManager::add_observation" << endl;
    for (auto & cal: calibrator)
      cerr << "\tname=" << cal->get_name() << " nchan=" << cal->get_nchan() << endl;
  }

  SystemCalibrator* model = get_calibrator (data);
  model->add_observation (data);

  // model is not in pole position, swap it in
  if (calibrator[0]->get_nchan() == 0 && model->get_nchan() > 0)
  {
    if (Archive::verbose > 1)
    {
      cerr << "SystemCalibratorManager::add_observation before swap:" << endl;
      for (auto & cal: calibrator)
        cerr << "\t" << cal->get_name() << endl;
    }

    for (auto & cal: calibrator)
    {
      if (cal == model)
      {
        if (Archive::verbose > 1)
          cerr << "SystemCalibratorManager::add_observation swapping " << model->get_name() 
              << " with " << calibrator[0]->get_name() << endl;
        std::swap(cal,calibrator[0]);
      }
    }

    if (Archive::verbose > 1)
    {
      cerr << "SystemCalibratorManager::add_observation after swap:" << endl;
      for (auto & cal: calibrator)
        cerr << "\t" << cal->get_name() << endl;
    }
  }

  if (!sharing_setup)
  {
    if (Archive::verbose > 1)
      cerr << "SystemCalibratorManager::add_observation setup sharing" << endl;
    for (unsigned i=1; i < calibrator.size(); i++)
      calibrator[i]->share (calibrator[0]);

    sharing_setup = true;
  }
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

//! Calibrate the the given archive using the current state of the model
void SystemCalibratorManager::precalibrate (Archive* data)
{
  SystemCalibrator* fiducial = get_model();
  SystemCalibrator* model = get_calibrator (data);

  unsigned nchan = fiducial->get_nchan();
  for (unsigned ichan=0; ichan < nchan; ichan++)
  {
    if (!fiducial->get_valid(ichan) && model->get_valid(ichan))
    {
      if (Archive::verbose > 1)
        cerr << "SystemCalibratorManager::precalibrate invalid ichan=" << ichan << " in fiducial model" << endl;
      model->set_valid (ichan, false, "invalid in fiducial model");
    }
  }

  model->precalibrate (data);
}

