/***************************************************************************
 *
 *   Copyright (C) 2004-2008 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/FITSArchive.h"
#include "Pulsar/ConfigurableProjectionExtension.h"
#include "CalibratorExtensionIO.h"

#include "psrfitsio.h"
#include "strutil.h"

// #define _DEBUG 1
#include "debug.h"

#include <stdlib.h>
#include <assert.h>

using namespace std;

void load_variances (fitsfile* fptr, Pulsar::ConfigurableProjectionExtension* cpe,
		     int nparam, vector<float>& data);

void load_covariances (fitsfile* fptr, Pulsar::ConfigurableProjectionExtension* cpe,
		       int ncovar, vector<float>& data);

void Pulsar::FITSArchive::load_ConfigurableProjectionExtension (fitsfile* fptr) try
{
  if (verbose == 3)
    cerr << "FITSArchive::load_ConfigurableProjectionExtension entered" << endl;
  
  // Move to the CFGPROJ HDU
  int status = 0;
  fits_movnam_hdu (fptr, BINARY_TBL, "CFGPROJ", 0, &status);
  
  if (status == BAD_HDU_NUM)
  {
    if (verbose == 3)
      cerr << "Pulsar::FITSArchive::load_ConfigurableProjectionExtension"
	" no CFGPROJ HDU" << endl;
    return;
  }

  if (status != 0)
    throw FITSError (status, "FITSArchive::load_ConfigurableProjectionExtension", 
		     "fits_movnam_hdu CFGPROJ");

  Reference::To<ConfigurableProjectionExtension> cpe = new ConfigurableProjectionExtension;

  string yaml = cpe->get_yaml();

  // Write YAML as long string
  char* comment = NULL;
  char* longstr = NULL;

  fits_read_key_longstr  (fptr, "YAML", &longstr, comment, &status);
  cpe->set_yaml (longstr);
  fits_free_memory (longstr, &status);

  // Get NPARAM 
  int nparam = 0;
  psrfits_read_key (fptr, "NPARAM", &nparam, 0, verbose == 3);
  if (nparam < 0)
    nparam = 0;

  int ncovar = 0;
  psrfits_read_key (fptr, "NCOVAR", &ncovar, 0, verbose == 3);

  vector<string> param_names (nparam);

  for (int iparam=0; iparam < nparam; iparam++)
  {
    string key = stringprintf ("PAR_%04d", iparam);
    string empty = "";
    psrfits_read_key (fptr, key.c_str(), &(param_names[iparam]),
		      empty, verbose == 3);
  }

  Pulsar::load (fptr, cpe);

  long dimension = cpe->get_nchan() * nparam;  
  
  if (dimension == 0)
  {
    if (verbose == 3)
      cerr << "FITSArchive::load_ConfigurableProjectionExtension CFGPROJ HDU"
	   << " contains no data. ConfigurableProjectionExtension not loaded" << endl;
    return;
  }

  unsigned ichan;

  for (ichan=0; ichan < cpe->get_nchan(); ichan++)
    if ( cpe->get_weight (ichan) == 0 )
    {
      if (verbose == 3)
        cerr << "FITSArchive::load_ConfigurableProjectionExtension ichan=" << ichan
             << " flagged invalid" << endl;
      cpe->set_valid (ichan, false);
    }

  vector<float> data (dimension);
  
  psrfits_read_col (fptr, "DATA", data);

  if (verbose == 3)
    cerr << "FITSArchive::load_ConfigurableProjectionExtension data read" << endl;
  
  int count = 0;
  for (ichan = 0; ichan < cpe->get_nchan(); ichan++)
    if (cpe->get_valid(ichan))
    {
      DEBUG ("FITSArchive::load_ConfigurableProjectionExtension ichan=" << ichan);
      bool valid = true;
      for (int j = 0; j < nparam; j++)
      {
	cpe->get_transformation(ichan)->set_param_name (j, param_names[j]);

	if (!isfinite(data[count]))
	  valid = false;
	else
        {
	  cpe->get_transformation(ichan)->set_param(j,data[count]);
          DEBUG ("\t" << j << " " << data[count]);
        }
	count++;
      }
      if (!valid)
	cpe->set_valid (ichan, false);
    }
    else
      count += nparam;

  assert (count == dimension);

  if (ncovar)
    load_covariances (fptr, cpe, ncovar, data);
  else
    load_variances (fptr, cpe, nparam, data);

  add_extension (cpe);
  
  if (verbose == 3)
    cerr << "FITSArchive::load_ConfigurableProjectionExtension exiting" << endl;

}
catch (Error& error)
{
  throw error += "FITSArchive::load ConfigurableProjectionExtension";
}

//
//
//
void load_variances (fitsfile* fptr, Pulsar::ConfigurableProjectionExtension* cpe,
		     int nparam, vector<float>& data)
{
  data.resize( nparam * cpe->get_nchan() );

  psrfits_read_col (fptr, "DATAERR", data);

  if (Pulsar::Archive::verbose > 2)
    cerr << "FITSArchive::load_ConfigurableProjectionExtension dataerr read" << endl;
  
  unsigned count = 0;

  for (unsigned ichan = 0; ichan < cpe->get_nchan(); ichan++)
  {
    if (cpe->get_valid(ichan))
    {
      bool valid = true;
      int zeroes = 0;

      for (int j = 0; j < nparam; j++)
      {
	float err = data[count];

	if (!isfinite(err))
	  valid = false;
	else
	  cpe->get_transformation(ichan)->set_variance (j,err*err);

	if (err == 0)
	  zeroes++;

	count++;	
      }

      if (zeroes == nparam)
      {
	if (Pulsar::Archive::verbose > 1)
	  cerr << "Pulsar::FITSArchive::load_ConfigurableProjectionExtension"
	    " WARNING\n  ichan=" << ichan << " flagged invalid:"
	    " zero error in all parameters" << endl;
	valid = false;
      }

      if (!valid)
	cpe->set_valid (ichan, false);

    }
    else
      count += nparam;
  }

  assert (count == data.size());
}

//
//
//
void load_covariances (fitsfile* fptr, Pulsar::ConfigurableProjectionExtension* cpe,
		       int ncovar, vector<float>& data)
{
  if (Pulsar::Archive::verbose > 2)
    cerr << "FITSArchive::load_ConfigurableProjectionExtension"
      " ncovar=" << ncovar << endl;

  unsigned nchan = cpe->get_nchan();

  data.resize( ncovar * nchan );

  psrfits_read_col (fptr, "COVAR", data);

  if (Pulsar::Archive::verbose > 2)
    cerr << "FITSArchive::load_ConfigurableProjectionExtension COVAR read" << endl;

  vector<double> covar (ncovar);
  unsigned count = 0;

  for (unsigned ichan = 0; ichan < nchan; ichan++)
  {
    if (!cpe->get_valid(ichan))
    {
      count += ncovar;
      continue;
    }

    DEBUG ("FITSArchive::load_ConfigurableProjectionExtension ichan=" << ichan);
    for (int j = 0; j < ncovar; j++)
    {
      DEBUG ("\t" << j << " " << data[count]);
      covar[j] = data[count];
      count++;
    }

    cpe->get_transformation(ichan)->set_covariance (covar);
  }

  assert (count == data.size());

  cpe->set_has_covariance( true );
}

