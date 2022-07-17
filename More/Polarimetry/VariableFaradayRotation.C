/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/VariableFaradayRotation.h"
#include "Pulsar/Integration.h"
#include "Pulsar/Faraday.h"
#include "Pulsar/AuxColdPlasmaMeasures.h"

using namespace Pulsar;

Jones<double> VariableFaradayRotation::get_transformation ()
{
  if (!built)
    build ();

  return transformation;
}

bool VariableFaradayRotation::required () const
{
  if (!built)
    build ();

  return is_required;
}

std::string VariableFaradayRotation::get_description() const
{
  if (!built)
    build ();

  return description;
}

void VariableFaradayRotation::set_ionospheric_rotation_measure (double RM)
{
  ionospheric_rotation_measure = RM;
  built = false;
}

void VariableFaradayRotation::set_interstellar_rotation_measure (double RM)
{
  ionospheric_rotation_measure = RM;
  built = false;
}

void VariableFaradayRotation::build () const try
{
  if (built)
    return;

  const Integration* integration = archive->get_Integration (subint);

  is_required = false;
  description = "";

  double iono_rm = ionospheric_rotation_measure;

  if (iono_rm != 0.0)
  {
    description += " correcting ionospheric Faraday rotation - RM=" + tostring(ionospheric_rotation_measure) + "\n";
    is_required = true;
  }
  else if (! integration->get_auxiliary_birefringence_corrected ())
  {
    const AuxColdPlasmaMeasures* aux = integration->get<AuxColdPlasmaMeasures> ();

    if (aux)
    {
      iono_rm = aux->get_rotation_measure();

      if (iono_rm != 0)
      {
        description += " correcting auxiliary Faraday rotation - RM=" + tostring(iono_rm) + "\n";
        is_required = true;
      }
    }
  }

  if (!iono_faraday)
    iono_faraday = new Calibration::Faraday;

  iono_faraday->set_rotation_measure( iono_rm );
  // correct ionospheric Faraday rotation wrt infinite frequency
  iono_faraday->set_reference_wavelength( 0.0 );

  double ism_rm = interstellar_rotation_measure;

  if (ism_rm != 0.0)
  {
    description += " correcting interstellar Faraday rotation - RM=" + tostring(ism_rm) + "\n";
    is_required = true;
  }
  else if (! archive->get_faraday_corrected ())
  {
    ism_rm = archive->get_rotation_measure ();

    if (ism_rm != 0)
    {
      description += " correcting interstellar Faraday rotation - (header) RM=" + tostring(ism_rm) + "\n";
      is_required = true;
    }
  }

  if (!ism_faraday)
    ism_faraday = new Calibration::Faraday;

  ism_faraday->set_rotation_measure( ism_rm );
  // correct interstellar Faraday rotation wrt centre frequency
  ism_faraday->set_reference_frequency( archive->get_centre_frequency() );

  transformation = Jones<double>::identity();

  iono_faraday->set_frequency( integration->get_centre_frequency(chan) );
  transformation *= iono_faraday->evaluate();

  ism_faraday->set_frequency( integration->get_centre_frequency(chan) );
  transformation *= ism_faraday->evaluate();

  built = true;
}
catch (Error& error)
{
  throw error += "VariableFaradayRotation::build";
}

void VariableFaradayRotation::set_chan (unsigned)
{
  // ignore (this override disables resetting the built flag)
}

