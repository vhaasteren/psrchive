/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/KnownVariableTransformation.h"

using namespace Pulsar;

void KnownVariableTransformation::set_nchan (unsigned nchan)
{
  xform.resize(nchan);
}

KnownVariableTransformation::Transformation*
KnownVariableTransformation::get_transformation (unsigned ichan)
{
  return &(xform.at(ichan));
}

MEAL::Argument::Value* KnownVariableTransformation::new_value ()
{
  return xform[chan].argument.new_Value( get_value() );
}
