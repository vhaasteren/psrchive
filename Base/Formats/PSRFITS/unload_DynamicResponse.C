/***************************************************************************
 *
 *   Copyright (C) 2024 Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/*
 * unload_DynamicResponse :: For storing dynamic frequency response data 
 * in DYN_RESP PSRFITS Binary Table
 */

#include "Pulsar/FITSArchive.h" 
#include "Pulsar/DynamicResponse.h" 

#include "psrfitsio.h" 
#include <stdlib.h>  

using namespace std;

void Pulsar::FITSArchive::unload (fitsfile* fptr, const DynamicResponse* response) 
{
  if (verbose > 2)
    cerr << "FITSArchive::unload DynamicResponse entered" << endl;
    
  unsigned nchan = response->get_nchan();
  unsigned ntime = response->get_ntime();
  unsigned npol = response->get_npol();
  unsigned ndat = response->get_data().size();

  if (verbose > 2) 
    cerr << "FITSArchive::unload DynamicResponse"
      " nchan=" << nchan <<
      " ntime=" << ntime <<
      " npol=" << npol <<
      " ndat=" << ndat << endl;

  if (ndat == 0)
  {
    if (verbose > 2)
      cerr << "Pulsar::FITSArchive::unload DynamicResponse - no data" << endl;
    return;
  }

  unsigned expected_ndat = nchan * ntime * npol;

  if (ndat != expected_ndat)
    throw Error (InvalidState, "Pulsar::FITSArchive::unload DynamicResponse",
                "invalid data dimensions: ndat=%u does not equal %u = nchan*ntime*npol=%u*%u*%u",
                ndat, expected_ndat, nchan, ntime, npol);

  // Move and Clear existing rows in DYN_RESP 
  psrfits_move_hdu (fptr, "DYN_RESP");

  psrfits_update_key (fptr, "NCHAN",    nchan);
  psrfits_update_key (fptr, "NTIME",    ntime);
  psrfits_update_key (fptr, "NPOL",     npol);
  psrfits_update_key (fptr, "CFREQ",    response->get_centre_frequency());
  psrfits_update_key (fptr, "BW",       response->get_bandwidth());
  psrfits_update_key (fptr, "NPOS_FIR", response->get_impulse_pos());
  psrfits_update_key (fptr, "NNEG_FIR", response->get_impulse_neg());

  double min_epoch = response->get_minimum_epoch().in_days();
  psrfits_update_key (fptr, "MINEPOCH", min_epoch);

  double max_epoch = response->get_maximum_epoch().in_days();
  psrfits_update_key (fptr, "MAXEPOCH", max_epoch);

  vector<unsigned> dimensions = { npol, nchan, ntime };
  psrfits_write_col (fptr, "DATA", 1, response->get_data(), dimensions);

  if (verbose > 2)       
    cerr << "FITSArchive::unload DynamicResponse exiting" << endl;
}

