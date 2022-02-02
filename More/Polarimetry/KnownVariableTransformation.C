/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/KnownVariableTransformation.h"

using namespace Pulsar;

//! Return a newly constructed Transformation instance
KnownVariableTransformation::Transformation*
KnownVariableTransformation::new_transformation ()
{
  return new KnownVariableTransformation::Transformation;
}

//! Return a newly constructed Argument::Value for the given Transformation
MEAL::Argument::Value*
KnownVariableTransformation::new_value (VariableTransformationManager::Transformation* xform)
{
  MEAL::Argument* argument = xform->get_argument ();
  KnownArgument* known = dynamic_cast<KnownArgument*> (argument);

  if (!known)
    throw Error (InvalidParam, "KnownVariableTransformation::new_value",
		 "Transformation does not have a KnownArgument");
						      
  return known->new_Value( get_transformation() );
}
