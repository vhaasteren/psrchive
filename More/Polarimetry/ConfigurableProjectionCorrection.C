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
#include "Pulsar/TransformationFactory.h"
#include "MEAL/NvariateScalarFactory.h"

#if HAVE_YAMLCPP
#include <yaml-cpp/yaml.h>
#endif

using namespace Pulsar;
using namespace std;

#if HAVE_YAMLCPP

//! Construct an object from a YAML::Node
template<typename T, typename Factory>
T* construct (const YAML::Node& node, Factory factory)
{
  if (!node.IsMap())
    throw Error (InvalidParam, "construct<T> (YAML::Node)",
                 "YAML::Node is not a Map");

  if ( !node["model"] )
    throw Error (InvalidParam, "construct<T> (YAML::Node)",
                 "map does not contain 'model'");

  YAML::Node model = node["model"];

  if (model.IsScalar())
    return factory( model.as<std::string>() );

  if (!model.IsMap())
    throw Error (InvalidParam, "construct<T> (YAML::Node)",
                 "YAML::Node for 'model' is neither a Scalar nor a Map");

  if ( !model["name"] )
    throw Error (InvalidParam, "ConfigurableProjectionCorrection::load",
                 "model map does not contain 'name'");

  T* object = factory( model["name"].as<std::string>() );

  auto interface = object->get_interface();

  for (auto it=model.begin(); it!=model.end(); ++it)
  {
    string key = it->first.as<string>();

    if (key == "name")
      continue;

    string value = it->second.as<string>();

    // cerr << "parsing key='" << key << "' value='" << value << "'" << endl;
    interface->set_value (key, value);
  }

  return object;
}

#endif

ConfigurableProjectionCorrection::ConfigurableProjectionCorrection (const string& filename)
{
#if HAVE_YAMLCPP

  YAML::Node node = YAML::LoadFile(filename);

  Calibration::TransformationFactory xform_factory;
  MEAL::Complex2* xform = construct<MEAL::Complex2> (node, xform_factory);

  MEAL::NvariateScalarFactory function_factory;

  for (auto it=node.begin(); it!=node.end(); ++it)
  {
    string key = it->first.as<string>();
    if ( key == "chain" )
    {
      YAML::Node chain = it->second;
      if (! chain.IsMap())
        throw Error (InvalidParam, "ConfigurableProjectionCorrection::load",
                     "YAML value for 'chain' is not a map");

      cerr << "parsing chain param" << endl;
      if ( !chain["param"] )
        throw Error (InvalidParam, "ConfigurableProjectionCorrection::load",
                     "YAML chain map does not contain 'param'");

      string param_name = chain["param"].as<string>();

      cerr << "parsing chain model" << endl;
      MEAL::Nvariate<MEAL::Scalar>* func = construct< MEAL::Nvariate<MEAL::Scalar> > (chain, function_factory);

      if ( !chain["args"] )
        throw Error (InvalidParam, "ConfigurableProjectionCorrection::load",
                     "YAML chain map does not contain 'args'");

      YAML::Node args = chain["args"];
      if (! args.IsSequence())
       throw Error (InvalidParam, "ConfigurableProjectionCorrection::load",
                     "YAML value for 'args' is not a map");

      if (args.size() != func->get_ndim())
        cerr << "WARNING: number of arguments = " << args.size() << " != "
                "number of dimensions = " << func->get_ndim() << endl;

      cerr << "parsing arguments" << endl;
      vector<string> params (args.size());
      for (unsigned i=0; i<args.size(); i++)
        params[i] = args[i].as<string>();
    }

  }

#else
  throw Error (InvalidState, "ConfigurableProjectionCorrection ctor",
               "yaml-cpp required and not available");
#endif
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
  auto xform = new ConfigurableProjectionCorrection::Transformation;

  // TODO: here is where the transformation is configured

  Calibration::VariableTransformation* cfg = xform->get_transformation ();

#if 0
  for (auto f : function)
  {
    chain->set_constraint (f.first, f.second);

    f.second->set_param (0, 0.0);
    f.second->set_infit (0, false);
  }
#endif

  return xform;
}

using Calibration::VariableTransformation;

//! Return a newly constructed Argument::Value for the given Transformation
MEAL::Argument::Value*
ConfigurableProjectionCorrection::new_value (VariableTransformationManager::Transformation* xform)
{
  MEAL::Argument* argument = xform->get_argument ();

  typedef MEAL::Axis< Calibration::VariableTransformation::Argument > VariableArgument;

  VariableArgument* vararg = dynamic_cast<VariableArgument*> (argument);

  if (!vararg)
    throw Error (InvalidParam, "ConfigurableProjectionCorrection::new_value",
		 "Transformation does not have a VariableArgument");

  Calibration::VariableTransformation::Argument arg;

  arg.correction = projection.get_transformation ();

  for (auto param : parameters)
  {
    unsigned index = param.first;

    unsigned narg = param.second.size();
    vector<double> args (narg);

    for (unsigned iarg=0; iarg < narg; iarg++)
      args[iarg] = get_value( param.second[iarg] );

    arg.arguments[index] = args;
  }

  return vararg->new_Value( arg );
}

//! Return the value associated with the parameter name
double ConfigurableProjectionCorrection::get_value (const std::string& name)
{
  return 0;
}


