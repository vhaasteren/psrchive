//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2019 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#ifndef __Pulsar_VariableProjectionCorrection_h
#define __Pulsar_VariableProjectionCorrection_h

#include "Pulsar/KnownVariableTransformation.h"
#include "Pulsar/ProjectionCorrection.h"

namespace Pulsar {

  //! Adapts a KnonwnVariableTransformation to a ProjectionCorrection
  class VariableProjectionCorrection : public KnownVariableTransformation
  {
    mutable ProjectionCorrection correction;
    mutable Jones<double> transformation;

    void build () const;

  public:

    //! Get the projection correction
    ProjectionCorrection* get_correction () { return &correction; }
    
    //! Get the transformation
    Jones<double> get_transformation ();

    //! Return true if the transformation is required
    bool required () const;

    //! Return the description
    std::string get_description () const;

    //! Set the frequency channel index
    void set_chan (unsigned);
  };
}

#endif
