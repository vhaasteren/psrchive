/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/VariableTransformation.h"

#include "Pauli.h"
#include "Error.h"

using namespace std;
using namespace Calibration;

// #define _DEBUG 1

VariableTransformation::VariableTransformation ()
{
}

VariableTransformation::VariableTransformation (const VariableTransformation& s)
{
  operator = (s);
}

//! Assignment Operator
const VariableTransformation& 
VariableTransformation::operator = (const VariableTransformation& s)
{
  if (&s == this)
    return *this;

  // clone the model
  if (s.model)
    set_model (s.model->clone());

  // clone any constraints
  for (auto f: s.function)
    set_constraint (f.first, f.second->clone());

  return *this;
}

VariableTransformation::~VariableTransformation ()
{
#if _DEBUG
  cerr << "VariableTransformation::dtor this=" << this << endl;
#endif
}

VariableTransformation* VariableTransformation::clone () const
{
  return new VariableTransformation (*this);
}

void VariableTransformation::set_model (MEAL::Complex2* _model)
{
  model = _model;
  chain.set_model (model);
}

MEAL::Complex2* VariableTransformation::get_model ()
{
  return model;
}

const MEAL::Complex2* VariableTransformation::get_model () const
{
  return model;
}

//! Set the multivariate function that constrains the specified parameter
void VariableTransformation::set_constraint (unsigned index, 
                                             MEAL::Multivariate<MEAL::Scalar>* func)
{
#if _DEBUG
  cerr << "VariableTransformation::set_constrain this=" << this <<
  " function=" << (void*) func;
  if (function)
    cerr << " " << func->get_name();
  cerr << endl;
#endif
  
  function[index] = func;
  chain.set_constraint (index, func);
}

void VariableTransformation::set_argument (const Argument& argset)
{
  if ( argset.arguments.size() != function.size() )
    throw Error (InvalidState, "VariableTransformation::set_argument",
                 "number of parameter sets = %d does not equal number of functions = %d",
                 argset.arguments.size(), function.size() );

  for (auto arg : argset.arguments)
  {
    // arg.first is the parameter index
    auto f = function.at( arg.first );

    // arg.second is the vector of arguments for each abscissa
    unsigned nargs = arg.second.size();

    for (unsigned iarg=0; iarg < nargs; iarg++)
      f->set_abscissa (iarg, arg.second[iarg]);
  }

  correction.set_value (argset.correction);
}

