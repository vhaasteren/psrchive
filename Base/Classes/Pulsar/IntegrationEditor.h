//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2024 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#ifndef __Pulsar_IntegrationEditor_h
#define __Pulsar_IntegrationEditor_h

#include "TextEditor.h"

namespace Pulsar {

  class Integration;

  //! Edit the metadata in a pulsar Integration
  class IntegrationEditor : public TextEditor<Integration>
  {

  public:

    //! Add the named extension to the given archive
    void add_extension (Integration*, const std::string& name);

    //! Remove the named extension from the given archive
    void remove_extension (Integration*, const std::string& name);

  };

}

#endif
