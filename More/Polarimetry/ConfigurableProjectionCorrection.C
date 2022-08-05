/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "Pulsar/ConfigurableProjectionCorrection.h"
#include "Pulsar/SingleAxis.h"
#include "MountProjection.h"

#if HAVE_YAMLCPP
#include <yaml-cpp/yaml.h>
#endif

using namespace Pulsar;
using namespace std;

ConfigurableProjectionCorrection::ConfigurableProjectionCorrection (const string& filename)
{
#if HAVE_YAMLCPP

  YAML::Node node = YAML::LoadFile(filename);

  unsigned nnode = 1;

  if (node.IsSequence())
  {
    cerr << filename << " contains a sequence" << endl;
    nnode = node.size();
  }

  for (unsigned inode=0; inode<nnode; inode++)
  {
    YAML::Node one;
    if (node.IsSequence())
      one = node[inode];
    else
      one = node;

#if 0

    if ( !one["M"] )
      throw Error (InvalidState, "SpectralKurtosis::load_configuration",
                   "M not specified for resolution[%u]", ires);

    resolution[ires].set_M( one["M"].as<unsigned>() );

    // the rest are optional

    if ( one["overlap"] )
      resolution[ires].noverlap = one["overlap"].as<unsigned>();

    if ( one["exclude"] )
      parse_ranges( one["exclude"], resolution[ires].exclude );

    if ( one["include"] )
      parse_ranges( one["include"], resolution[ires].include );

#endif

  }

#else
  throw Error (InvalidState, "ConfigurableProjectionCorrection ctor",
               "yaml-cpp required and not available");
#endif
}

ConfigurableProjectionCorrection::ConfigurableTransformation::ConfigurableTransformation ()
{
  auto chain = new MEAL::ChainRule<MEAL::Complex2>;
  auto single = new Calibration::SingleAxis;

  // disable variation of absolute gain
  single->set_infit (0, false);
  
  chain->set_model (single);

  for (auto f : function)
  {
    chain->set_constraint (f.first, f.second);
  
    f.second->set_param (0, 0.0);
    f.second->set_infit (0, false);
  }

  add_model (chain);
  
  // corrects the known projection transformation
  add_model (&correction);
}

ConfigurableProjectionCorrection::ConfigurableTransformation::~ConfigurableTransformation ()
{
}

void ConfigurableProjectionCorrection::ConfigurableTransformation::set_argument
(const Argument& arg)
{
  unsigned nargs = arg.arguments.size();

  for (auto f : function)
  { 
    for (unsigned iarg=0; iarg < nargs; iarg++)
    {
      f.second->set_abscissa (iarg, arg.arguments[iarg]);
    }
  }
 
  correction.set_value (arg.correction);
}

//! Set the Archive for which a tranformation will be computed
void ConfigurableProjectionCorrection::set_archive (const Archive* _archive)
{
  VariableTransformationManager::set_archive (_archive);
  projection.set_archive (_archive);
}

//! Set the sub-integration for which a tranformation will be computed
void ConfigurableProjectionCorrection::set_subint (unsigned _subint)
{
  VariableTransformationManager::set_subint (_subint);
  projection.set_subint (_subint);
}

//! Set the frequency channel for which a tranformation will be computed
void ConfigurableProjectionCorrection::set_chan (unsigned _chan)
{
  VariableTransformationManager::set_chan (_chan);
  projection.set_chan (_chan);
}

//! Return a newly constructed Transformation instance
ConfigurableProjectionCorrection::Transformation*
ConfigurableProjectionCorrection::new_transformation ()
{
  return new ConfigurableProjectionCorrection::Transformation;
}

//! Return a newly constructed Argument::Value for the given Transformation
MEAL::Argument::Value*
ConfigurableProjectionCorrection::new_value (VariableTransformationManager::Transformation* xform)
{
  MEAL::Argument* argument = xform->get_argument ();
  ConfigurableArgument* cfgarg = dynamic_cast<ConfigurableArgument*> (argument);

  if (!cfgarg)
    throw Error (InvalidParam, "ConfigurableProjectionCorrection::new_value",
		 "Transformation does not have a ConfigurableArgument");

  Argument arg;

  arg.correction = projection.get_transformation ();

  unsigned narg = parameters.size();
  arg.arguments.resize( narg );
  for (unsigned iarg=0; iarg < narg; iarg++)
    arg.arguments[iarg] = get_value( parameters[iarg] );

  return cfgarg->new_Value( arg );
}

//! Return the value associated with the parameter name
double ConfigurableProjectionCorrection::get_value (const std::string& name)
{
  return 0;
}


