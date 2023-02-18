/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/ConfigurableProjectionExtension.h"
#include "Pulsar/ConfigurableProjection.h"

using namespace std;

void copy_name (MEAL::Complex2* to,
		const Pulsar::ConfigurableProjectionExtension::Transformation* from)
{
  for (unsigned i=0; i<to->get_nparam(); i++)
  {
    string param_name = from->get_param_name(i);
    if (param_name != "" && param_name != to->get_param_name(i))
      throw Error (InvalidState, "copy_name",
		   "iparam=%d name=%s != model=%s",
		   i, param_name.c_str(), to->get_param_name(i).c_str());
  }
}

void copy_name (Pulsar::ConfigurableProjectionExtension::Transformation* to,
		const MEAL::Complex2* from)
{
  unsigned nparam = from->get_nparam();
  to->set_nparam( nparam );
  for (unsigned i=0; i<nparam; i++)
  {
    to->set_param_name(i, from->get_param_name(i));
    to->set_param_description(i, from->get_param_description(i));
  }
}

template<class T, class F>
void copy (T* to, const F* from)
{
  copy_name (to, from);

  if (to->get_nparam() != from->get_nparam())
    throw Error (InvalidParam, "copy<To,From>",
		 "to nparam=%d != from nparam=%d",
		 to->get_nparam(), from->get_nparam());

  for (unsigned i=0; i<to->get_nparam(); i++)
    to->set_Estimate(i, from->get_Estimate(i));
}

//! Construct from a ConfigurableProjection instance
Pulsar::ConfigurableProjectionExtension::ConfigurableProjectionExtension
(const ConfigurableProjection* calibrator) try
  : Archive::Extension ("ConfigurableProjectionExtension") 
{
  if (!calibrator)
    throw Error (InvalidParam, "", "null ConfigurableProjection*");

  init ();

  // if (Calibrator::verbose > 2)
    cerr << "Pulsar::ConfigurableProjectionExtension "
         "construct from ConfigurableProjection with config=\n" 
         << calibrator->get_configuration() << endl;

  set_configuration( calibrator->get_configuration() );

  has_covariance = false; // calibrator->has_covariance ();

  bool first = true;

  unsigned nchan = calibrator->get_nchan ();
  set_nchan (nchan);

  // if (Calibrator::verbose > 2)
    cerr << "Pulsar::ConfigurableProjectionExtension nchan=" << nchan << endl;

  for (unsigned ichan=0; ichan < nchan; ichan++)
  {
    if ( ! calibrator->get_transformation_valid(ichan) )
    {
      if (Calibrator::verbose > 2)
	cerr << "Pulsar::ConfigurableProjectionExtension ichan=" << ichan 
	     << " flagged invalid" << endl;
      
      set_valid (ichan, false);
      continue;
    }

    const ConfigurableProjection::Transformation* transformation = calibrator->get_transformation(ichan);
    const Calibration::VariableTransformation* xform = transformation->get_transformation();

    copy( get_transformation(ichan), xform );

    set_valid (ichan, true);

    if (first)
    {
      nparam = xform->get_nparam();

      // if (Calibrator::verbose > 2)
      {
        const MEAL::Function* f = calibrator->get_transformation(ichan)->get_transformation();
        for (unsigned i=0; i<f->get_nparam(); i++)
	  cerr << "Pulsar::ConfigurableProjectionExtension name[" << i << "]=" 
	       << f->get_param_name(i) << endl;
      }
    }
    else if (get_nparam() != xform->get_nparam())
    {
      throw Error (InvalidParam, "Pulsar::ConfigurableProjectionExtension ctor",
                 "nparam=%d != Trnaformation[ichan=%d]:nparam=%d",
                 get_nparam(), ichan, xform->get_nparam());
    }

    first = false;

#if 0
    if ( has_covariance )
    {
      calibrator->get_covariance( ichan, covariance );
      get_transformation(ichan)->set_covariance( covariance );
    }
#endif

  }
}
catch (Error& error)
{
  throw error += "Pulsar::ConfigurableProjectionExtension (ConfigurableProjection*)";
}

#if 0

//! Return a new MEAL::Complex2 instance, based on type attribute
MEAL::Complex2*
Pulsar::new_transformation (const ConfigurableProjectionExtension* ext, unsigned ichan)
try
{
  if( !ext->get_valid(ichan) )
    return 0;

  MEAL::Complex2* xform = new_transformation( ext->get_type() );

  if (Calibrator::verbose)
    cerr << "Pulsar::new_transformation name=" << xform->get_name() << endl;

  const ConfigurableProjectionExtension::Transformation* info;
  info = ext->get_transformation(ichan);
  
  copy( xform, info );

  for (unsigned iparam=0; iparam < xform->get_nparam(); iparam++)
    if (info->get_variance(iparam) == 0)
      xform->set_infit (iparam, false);
  
  return xform;
}
catch (Error& error)
{
  throw error += "Pulsar::new_transformation";
}

#endif

