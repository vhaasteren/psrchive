/***************************************************************************
 *
 *   Copyright (C) 2009 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/ProcHistory.h"

using Pulsar::ProcHistory;

ProcHistory::Interface::Interface( ProcHistory *s_instance )
{
  if( s_instance )
    set_instance( s_instance );

  add( &ProcHistory::get_nrow, "nrow", "Number of rows in history" );
  add( &ProcHistory::get_last_nbin_prd, "nbin_prd", "Nr of bins per period" );
  add( &ProcHistory::get_last_tbin, "tbin", "Time per bin or sample" );
  add( &ProcHistory::get_last_chan_bw, "chan_bw", "Channel bandwidth" );
  add( &ProcHistory::get_cal_file, "cal_file", "Calibrator filename" );
}


ProcHistory::row::Interface::Interface( ProcHistory::row *s_instance )
{
  if( s_instance )
    set_instance( s_instance );

  add( &ProcHistory::row::get_date_pro, "date", "Processing date and time" );
  add( &ProcHistory::row::get_proc_cmd, "cmd", "Processing command and arguments" );

  add( &ProcHistory::row::get_nsub, "nsub", "Number of sub-integrations" );
  add( &ProcHistory::row::get_npol, "npol", "Number of polarisations" );
  add( &ProcHistory::row::get_nchan, "nchan", "Number of frequency channels" );
  add( &ProcHistory::row::get_nbin, "nbin", "Number of pulse phase bins" );
  add( &ProcHistory::row::get_nbin_prd, "nbinP", "Number of phase bins per period" );

  add( &ProcHistory::row::get_tbin, "tbin", "Time spanned by one phase bin" );
  add( &ProcHistory::row::get_ctr_freq, "freq", "Centre frequency" );
  add( &ProcHistory::row::get_chan_bw, "chan_bw", "Channel bandwdith" );
  add( &ProcHistory::row::get_ref_freq, "ref_freq", "Reference frequency" );

  add( &ProcHistory::row::get_pr_corr, "prc", "Projection of receptors onto sky corrected" );
  add( &ProcHistory::row::get_fd_corr, "fdc", "Feed basis correction applied" );
  add( &ProcHistory::row::get_be_corr, "bec", "Backend correction applied" );
  add( &ProcHistory::row::get_cal_mthd, "cal_mthd", "Calibration method" );
  add( &ProcHistory::row::get_cal_file, "cal_file", "Name of gain calibration file" );

  add( &ProcHistory::row::get_dispersion_measure, "dm", "Dispersion measure" );
  add( &ProcHistory::row::get_dedisp, "dmc", "Dispersive delay corrected" );
  add( &ProcHistory::row::get_dds_mthd, "dm_mthd", "Dispersion correction method" );
  add( &ProcHistory::row::get_aux_dm_model, "aux_dm_model", "Auxiliary dispersion model" );
  add( &ProcHistory::row::get_aux_dm_corr, "aux_dmc", "Auxiliary dispersive delay corrected" );

  add( &ProcHistory::row::get_rotation_measure, "rm", "Rotation measure" );
  add( &ProcHistory::row::get_rm_corr, "rmc", "Faraday rotation corrected" );
  add( &ProcHistory::row::get_aux_rm_model, "aux_rm_model", "Auxiliary Faraday rotation model" );
  add( &ProcHistory::row::get_aux_rm_corr, "aux_rmc", "Auxiliary Faraday rotation corrected" );

  add( &ProcHistory::row::get_sc_mthd, "sc_mthd", "Scattered power correction method" );
  add( &ProcHistory::row::get_rfi_mthd, "rfi_mthd", "RFI excision method" );

  add( &ProcHistory::row::get_pol_type, "state", "Signal state" );
  add( &ProcHistory::row::get_scale, "scale", "Units (FluxDen/RefFlux/Jansky)" );
}
