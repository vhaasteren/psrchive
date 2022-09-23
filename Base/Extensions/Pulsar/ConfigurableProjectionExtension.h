//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

// psrchive/Base/Extensions/Pulsar/ConfigurableProjectionExtension.h

#ifndef __ConfigurableProjectionExtension_h
#define __ConfigurableProjectionExtension_h

#include "Pulsar/ArchiveExtension.h"
#include "Pulsar/ProjectionType.h"

namespace Pulsar {

  //! Stores Configurable Polarization Projection information
  /*! This Archive::Extension class implements the storage of
    Configurable Projection data, enabling such a calibrator to be
    constructed from file. */
  class ConfigurableProjectionExtension : public Archive::Extension
  {   
  public:
    
    //! Default constructor
    ConfigurableProjectionExtension
    (const char* name = "ConfigurableProjectionExtension");

    //! Copy constructor
    ConfigurableProjectionExtension (const ConfigurableProjectionExtension&);

    //! Operator =
    const ConfigurableProjectionExtension&
    operator= (const ConfigurableProjectionExtension&);

    //! Destructor
    ~ConfigurableProjectionExtension ();

    //! Clone method
    ConfigurableProjectionExtension* clone () const
    { return new ConfigurableProjectionExtension( *this ); }

    //! Return a short name
    std::string get_short_name () const { return "pcmcfg"; }

    //! Text interface to ConfigurableProjectionExtension extension
    class Interface
      : public TextInterface::To<ConfigurableProjectionExtension>
    {
    public:
      Interface (ConfigurableProjectionExtension* = 0);
    };
    
    //! Return a text interfaces that can be used to access this instance
    TextInterface::Parser* get_interface();

    class Parameter : public Reference::Able
    {
    public:

      //! Text interface to ConfigurableProjectionExtension::Parameter
      class Interface
	: public TextInterface::To<Parameter>
      {
      public:
	Interface (Parameter* = 0);
      };

#if 0
      unsigned iparam;
      unsigned get_iparam () const { return iparam; }

      std::string interpolator;
#endif
    };

    void add_parameter (Parameter* p) { parameter.push_back(p); }
    unsigned get_nparam () const { return parameter.size(); }
    
    Parameter* get_parameter (unsigned iparam)
    { return parameter.at(iparam); }
    const Parameter* get_parameter (unsigned iparam) const
    { return parameter.at(iparam); }
    
    protected:

    std::vector< Reference::To<Parameter> > parameter;
  };

}

#endif
