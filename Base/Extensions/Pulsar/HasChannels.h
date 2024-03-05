//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2024 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

// psrchive/Base/Extensions/Pulsar/HasChannels.h

#ifndef __Base_Extensions_Pulsar_HasChannels_h
#define __Base_Extensions_Pulsar_HasChannels_h

#include "Pulsar/ArchiveExtension.h"

namespace Pulsar {
  
  //! Archive extensions that contain information for each frequency channel
  /*! The number of channels should match those of each Integration and other HasChannel extensions. */
  class HasChannels : public Pulsar::Archive::Extension {
    
  public:

    HasChannels (const char* name) : Archive::Extension(name) {}
    
    //! Set the number of frequency channels
    virtual void set_nchan (unsigned nchan) = 0;
    //! Get the number of frequency channels
    virtual unsigned get_nchan () const = 0;

    //! Remove the inclusive range of channels
    virtual void remove_chan (unsigned first, unsigned last) = 0;
  };

}

#endif
