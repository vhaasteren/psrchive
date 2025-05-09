//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#ifndef __Pulsar_VariableFaradayRotation_h
#define __Pulsar_VariableFaradayRotation_h

#include "Pulsar/KnownVariableTransformation.h"
#include "Pulsar/Faraday.h"

namespace Pulsar {

  //! Encapsulates known interstellar and ionospheric Faraday rotation
  class VariableFaradayRotation : public KnownVariableTransformation
  {
    // ionospheric Faraday rotation transformation
    mutable Reference::To<Calibration::Faraday> iono_faraday;

    // ionospheric Faraday rotation measure
    double ionospheric_rotation_measure;

    // interstellar Faraday rotation transformation
    mutable Reference::To<Calibration::Faraday> ism_faraday;

    // interstellar Faraday rotation measure
    double interstellar_rotation_measure;

    mutable LabelledJones<double> transformation;
    mutable bool is_required;
    mutable std::string description;
    void build () const;

  public:

    VariableFaradayRotation ();
 
    //! Get the transformation
    LabelledJones<double> get_transformation () override;

    //! Set the ionospheric Faraday rotation measure
    void set_ionospheric_rotation_measure (double);

    //! Set the interstellar Faraday rotation measure
    void set_interstellar_rotation_measure (double);

    //! Return true if the transformation is required
    bool required () const; 

    //! Return the description
    std::string get_description () const;

  };
}

#endif
