/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include <Pulsar/DataSetManager.h>
#include <Pulsar/ArchiveMatch.h>
#include <Pulsar/Profile.h>

#include "debug.h"

using namespace Pulsar;
using namespace std;

DataSet::DataSet ()
{
  total_integration_length = 0;
}

//! Return true if data can be added to this data set
bool DataSet::matches (const Archive* archive)
{
  // empty, so anything matches
  if (data.size() == 0)
    return true;

  Archive::Match test;
  test.set_check_centre_frequency (true);
  test.set_check_bandwidth (true);
  test.set_check_source (true);

  bool result = test.match (data[0], archive);

  if (!result)
    DEBUG( "DataSet::matches " << data[0]->get_filename() << " does not match " << archive->get_filename() << " " << test.get_reason());

  return result;
}
  
//! Add data to this data set
void DataSet::add (const Archive* archive)
{
  if (!matches (archive))
    throw Error (InvalidParam, "DataSet::add", "archive does not match");

  name = archive->get_source();

  total_integration_length += archive->integration_length ();

  MJD start = archive->start_time();
  MJD end = archive->end_time();

  if (data.size() == 0 || start < start_time)
    start_time = start;
  if (data.size() == 0 || end > end_time)
    end_time = end;

  data.push_back (archive);
}

//! Get the epoch of the first observation
MJD DataSet::get_start_epoch () const
{
  return start_time;
}

//! Get the epoch of the last observation
MJD DataSet::get_end_epoch () const
{
  return end_time;
}






//! Default constructor
DataSetManager::DataSetManager ()
{
  polncal_hours = 0;
  fluxcal_days = 7.0;
  check_coordinates = true;

  npolncal = 0;
}

//! Add to the array of system calibrators
void DataSetManager::manage (DataSet* ds)
{
  datasets.push_back (ds);
}

//! Return the reference epoch of the calibration experiment
MJD DataSetManager::get_epoch () const
{
  return 0.0;
}

//! Get the data set that matches the archive
DataSet* DataSetManager::get (const Archive* data)
{
  for (auto ds: datasets)
    if (ds->matches(data))
      return ds;

  throw Error (InvalidState, "DataSetManager::get",
	       "no data set matches " + data->get_filename());
}

//! Add archive to the appropriate data set
void DataSetManager::add (const Archive* data)
{
  MJD start = data->start_time();
  MJD end = data->end_time();

  if (datasets.size() == 0 || start < start_time)
    start_time = start;
  if (datasets.size() == 0 || end > end_time)
    end_time = end;
  
  for (auto ds: datasets)
    if (ds->matches(data))
    {
      ds->add(data);
      return;
    }

  cerr << "DataSetManager::add starting new data set name="
       << data->get_source() << endl;

  DataSet* ds = new DataSet;
  ds->add(data);
  
  manage(ds);
}

//! Get the epoch of the first observation
MJD DataSetManager::get_start_epoch () const
{
  return 0.0;
}

//! Get the epoch of the last observation
MJD DataSetManager::get_end_epoch () const
{
  return 0.0;
}


void DataSetManager::load ( vector<string>& filenames )
{
  Pulsar::Profile::no_amps = true;

  for (unsigned ifile=0; ifile < filenames.size(); ifile++) try
  {
    Reference::To<Pulsar::Archive> archive;
    archive = Pulsar::Archive::load( filenames[ifile] );
    add (archive);
  }
  catch (Error& error)
  {
    cerr << "pcm: get_span() error while handling " << filenames[ifile]
         << endl << "\t" << error.get_message() << endl;

    filenames.erase( filenames.begin() + ifile );
    ifile --;
  }

  Pulsar::Profile::no_amps = false;
}

void DataSetManager::add_polncals (DataSet* dataset)
{
  if (dataset->data.size() == 0)
    throw Error (InvalidState, "DataSet::add_polncals",
		 "no data");
  
  MJD mid = 0.5 * (dataset->end_time + dataset->start_time);
  MJD span = dataset->end_time - dataset->start_time;
  double span_hours = span.in_days() * 24.0;
  double search_hours = 0.5*span_hours + polncal_hours;

  cerr << "DataSetManager::find_polncals"
    " searching for reference source observations within " << search_hours
       << " hours of midtime for " << dataset->name << endl;

  char buffer[256];
  cerr << "DataSetManager::find_polncals midtime = "
       << mid.datestr (buffer, 256, "%Y-%m-%d-%H:%M:00") << endl;

  assert (dataset->data.size() > 0);
  const Archive* archive = dataset->data[0];
  
  Pulsar::Database::Criteria criteria;
  criteria = database->criteria (archive, Signal::PolnCal);
  criteria.entry.time = mid;
  criteria.check_coordinates = check_coordinates;
  criteria.minutes_apart = search_hours * 60.0;

  vector<Pulsar::Database::Entry> cals;
  database->all_matching (criteria, cals);

  if (cals.size() == 0)
  {
    cerr << "DataSetManager::find_polncals no PolnCal observations found;"
      " closest match was \n\n" << database->get_closest_match_report ()
	 << endl;

    return;
  }
    
  sort (cals.begin(), cals.end());
  
  for (auto cal: cals)
  {
    string filename = database->get_filename( cal );
    cerr << "DataSetManager::find_polncals " << cal.filename << endl;
    dataset->calibrator_filenames.push_back (filename);
    npolncal ++;
  }
}

void DataSetManager::find_poln_calibrators ()
{
  for (auto ds: datasets)
    add_polncals (ds);
}

void DataSetManager::find_on_flux_calibrators ()
{
  find_flux_calibrators (Signal::FluxCalOn, "on");
}

void DataSetManager::find_off_flux_calibrators ()
{
  find_flux_calibrators (Signal::FluxCalOff, "off");
}

void DataSetManager::find_flux_calibrators  (Signal::Source obsType,
					     const char* short_name)
{
  double span_days = (end_time - start_time).in_days();
  double search_days = 0.5*span_days + fluxcal_days;

  assert (datasets.size() > 0);
  DataSet* fiducial = datasets[0];
  assert (fiducial->data.size() > 0);
  const Archive* archive = fiducial->data[0];
  
  Pulsar::Database::Criteria criteria;
  criteria= database->criteria (archive, obsType);
  criteria.check_coordinates = false;
  criteria.minutes_apart = search_days * 24.0 * 60.0;
  
  cerr << "DataSetManager::find_flux_calibrators"
    " searching for " << short_name << "-source flux calibrator observations"
    " within " << search_days << " days of midtime" << endl;

  vector<Pulsar::Database::Entry> cals;
  database->all_matching (criteria, cals);
    
  if (cals.size() == 0)
    cerr << "DataSetManager::find_flux_calibrators"
      " no " << obsType << " observations found; closest match was \n\n"
	 << database->get_closest_match_report () << endl;

  sort (cals.begin(), cals.end());
  
  for (unsigned i = 0; i < cals.size(); i++)
  {
    string filename = database->get_filename( cals[i] );
    cerr << "DataSetManager::find_flux_calibrators adding "
	 << cals[i].filename << endl;
    fiducial->calibrator_filenames.push_back (filename);
  }
}

