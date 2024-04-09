/***************************************************************************
 *
 *   Copyright (C) 2007-2010 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

// #define _DEBUG 1

#include "Pulsar/Generator.h"
#include "Predict.h"
#include "lazy.h"
#include "debug.h"

#include <cassert>

using namespace std;

std::ostream& Pulsar::operator<< (std::ostream& ostr, Predictor::Policy p)
{
  if (p == Pulsar::Predictor::Input)
    return ostr << "input";

  if (p == Pulsar::Predictor::Default)
    return ostr << "default";

  if (p == Pulsar::Predictor::Ephemeris)
    return ostr << "ephem";

  return ostr;
}

std::istream& Pulsar::operator>> (std::istream& istr, Predictor::Policy& p)
{
  std::string policy;
  istr >> policy;

  if (policy == "input")
    p = Pulsar::Predictor::Input;
  else if (policy == "default")
    p = Pulsar::Predictor::Default;
  else if (policy == "ephem")
    p = Pulsar::Predictor::Ephemeris;
  else
    istr.setstate (std::istream::failbit);

  return istr;
}

LAZY_GLOBAL(Pulsar::Predictor, \
	    Configuration::Parameter<Pulsar::Predictor::Policy>, \
	    policy, Pulsar::Predictor::Ephemeris );

LAZY_GLOBAL(Pulsar::Generator, \
	    Configuration::Parameter<Pulsar::Generator*>, \
	    default_generator, new Tempo::Predict);

Pulsar::Generator* Pulsar::Generator::get_default ()
{
  Pulsar::Generator* generator = get_default_generator().get_value();
  DEBUG("Pulsar::Generator::get_default default_generator=" << generator);
  assert (generator != nullptr);
  return generator->clone ();
}

