/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/NancayProjectionCorrection.h"
#include "Pulsar/SingleAxis.h"
#include "MountProjection.h"

using namespace Pulsar;
using namespace std;

Poly2D::Poly2D (unsigned ncoeff, unsigned mcoeff)
{
  poly.resize (ncoeff);
  set_model (&poly);

  coefficients.resize (ncoeff - 1);
  for (unsigned i=0; i < coefficients.size(); i++)
  {
    coefficients[i].resize (mcoeff);
    set_constraint (i+1, &coefficients[i]);
  }
}
   
//! Set the abscissa value
void Poly2D::set_abscissa (unsigned dim, double value)
{
  if (dim == 0)
    poly.set_abscissa (value);
  else if (dim == 1)
    for (unsigned i=0; i < coefficients.size(); i++)
      coefficients[i].set_abscissa (value);
  else
    throw Error (InvalidParam, "Poly2D::set_abscissa",
		 "invalid dim=%u > 1", dim);
}

//! Set the abscissa offset
void Poly2D::set_abscissa_offset (unsigned dim, double offset)
{
  if (dim == 0)
    poly.set_abscissa_offset (offset);
  else if (dim == 1)
    for (unsigned i=0; i < coefficients.size(); i++)
      coefficients[i].set_abscissa_offset (offset);
  else
    throw Error (InvalidParam, "Poly2D::set_abscissa",
		 "invalid dim=%u > 1", dim);
}

NancayProjectionCorrection::NancayTransformation::NancayTransformation ()
: diff_phase (3, 2),      // quadratic in hour angle, linear in declination
  diff_gain  (3, 2)
{
  auto chain = new MEAL::ChainRule<MEAL::Complex2>;
  auto single = new Calibration::SingleAxis;

  // disable variation of absolute gain
  single->set_infit (0, false);
  
  chain->set_model (single);
  
  chain->set_constraint (1, &diff_gain);
  diff_gain.set_param (0, 0.0);
  diff_gain.set_infit (0, false);

  chain->set_constraint (2, &diff_phase);
  diff_phase.set_param (0, 0.0);
  diff_phase.set_infit (0, false);

  add_model (chain);
  
  // corrects the known Kraus-type::parallactic_angle and Pointing::feed_angle
  add_model (&correction);
}

NancayProjectionCorrection::NancayTransformation::~NancayTransformation ()
{
}

void NancayProjectionCorrection::NancayTransformation::set_argument
(const Argument& arg)
{
  diff_gain.set_abscissa (0, arg.hour_angle);
  diff_gain.set_abscissa (1, arg.declination);

  diff_phase.set_abscissa (0, arg.hour_angle);
  diff_phase.set_abscissa (1, arg.declination);
  
  correction.set_value (arg.correction);
}

//! Set the Archive for which a tranformation will be computed
void NancayProjectionCorrection::set_archive (const Archive* _archive)
{
  VariableTransformationManager::set_archive (_archive);
  projection.set_archive (_archive);
}

//! Set the sub-integration for which a tranformation will be computed
void NancayProjectionCorrection::set_subint (unsigned _subint)
{
  VariableTransformationManager::set_subint (_subint);
  projection.set_subint (_subint);
}

//! Set the frequency channel for which a tranformation will be computed
void NancayProjectionCorrection::set_chan (unsigned _chan)
{
  VariableTransformationManager::set_chan (_chan);
  projection.set_chan (_chan);
}

//! Return a newly constructed Transformation instance
NancayProjectionCorrection::Transformation*
NancayProjectionCorrection::new_transformation ()
{
  return new NancayProjectionCorrection::Transformation;
}

//! Return a newly constructed Argument::Value for the given Transformation
MEAL::Argument::Value*
NancayProjectionCorrection::new_value (VariableTransformationManager::Transformation* xform)
{
  MEAL::Argument* argument = xform->get_argument ();
  NancayArgument* nancay = dynamic_cast<NancayArgument*> (argument);

  if (!nancay)
    throw Error (InvalidParam, "NancayProjectionCorrection::new_value",
		 "Transformation does not have a NancayArgument");

  Argument arg;

  arg.correction = projection.get_transformation ();
  arg.hour_angle = projection.get_correction()->get_mount()->get_hour_angle();
  arg.declination = archive->get_coordinates().dec().getRadians();

  return nancay->new_Value( arg );
}
