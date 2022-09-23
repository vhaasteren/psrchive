/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/ConfigurableProjectionExtension.h"
#include "templates.h"

using namespace Pulsar;

//! Default constructor
ConfigurableProjectionExtension::ConfigurableProjectionExtension
(const char* name) : Extension (name)
{
}

//! Copy constructor
ConfigurableProjectionExtension::ConfigurableProjectionExtension
(const ConfigurableProjectionExtension& copy)
 : Extension (copy.get_extension_name().c_str())
{
  operator = (copy);
}

//! Operator =
const ConfigurableProjectionExtension&
ConfigurableProjectionExtension::operator= (const ConfigurableProjectionExtension& copy)
{
  if (this == &copy)
    return *this;

  parameter = copy.parameter;
 
  return *this;
}

//! Destructor
ConfigurableProjectionExtension::~ConfigurableProjectionExtension ()
{
}

ConfigurableProjectionExtension::Parameter::Interface::Interface
(ConfigurableProjectionExtension::Parameter *s_instance)
{
  if( s_instance )
    set_instance( s_instance );

#if 0

  add( &ConfigurableProjectionExtension::Parameter::get_code,
       "code", "Calibration model code" );

  add( &ConfigurableProjectionExtension::Parameter::get_iparam,
       "index", "Calibration model index" );

#endif
}


// Text interface to a ConfigurableProjectionExtension extension
ConfigurableProjectionExtension::Interface::Interface
  (ConfigurableProjectionExtension *s_instance)
{
  if( s_instance )
    set_instance( s_instance );

#if 0

  typedef ConfigurableProjectionExtension::Parameter*
  (ConfigurableProjectionExtension::*get_mutable) (unsigned);
  
  import ( "param", new Parameter::Interface(),
	   (get_mutable) &ConfigurableProjectionExtension::get_parameter,
	   &ConfigurableProjectionExtension::get_nparam );

#endif
}

//! Return a text interfaces that can be used to access this instance
TextInterface::Parser* ConfigurableProjectionExtension::get_interface()
{
  return new ConfigurableProjectionExtension::Interface( this );
}


