//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2004 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

// psrchive/Base/Classes/Pulsar/ArchiveInterface.h

#ifndef __Pulsar_ArchiveTI_h
#define __Pulsar_ArchiveTI_h

#include "Pulsar/Archive.h"
#include "TextInterface.h"

namespace Pulsar
{

  //! Provides a text interface to get and set Archive attributes
  class Archive::Interface : public TextInterface::To<Archive>
  {

  public:

    //! Get the number of Archive::Interface instances
    static unsigned get_instance_count ();

    //! Default constructor that takes an optional instance
    Interface ( Archive* = 0 );

    //! Desctructor
    ~Interface();

    //! Set the instance to which this interface interfaces
    void set_instance (Archive*) ;

    //! clone this text interface
    TextInterface::Parser *clone();
    
    //! Get the interface name
    std::string get_interface_name() const { return "Archive::Interface"; }

  };

}


#endif
