/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "MEAL/Polynomial2D.h"

using namespace std;
 
void MEAL::Polynomial2D::init()
{
  chain.set_model ( &poly );

  copy_parameter_policy  (&chain);
  copy_evaluation_policy (&chain);
}

MEAL::Polynomial2D::Polynomial2D (unsigned ncoeff, unsigned mcoeff)
: Multivariate<Scalar> (2)
{
  init ();

  if (ncoeff == 0 || mcoeff == 0)
    return;

  resize (ncoeff, mcoeff);
}

void MEAL::Polynomial2D::resize (unsigned ncoeff, unsigned mcoeff)
{
  poly.resize (ncoeff);

  if (ncoeff == 0)
    return;

  coefficients.resize (ncoeff - 1);
  for (unsigned i=0; i < coefficients.size(); i++)
  {
    coefficients[i].resize (mcoeff);
    chain.set_constraint (i+1, &coefficients[i]);
  }
}

//! Set the abscissa value
void MEAL::Polynomial2D::set_abscissa_value (unsigned dim, double value)
{
  Multivariate<Scalar>::set_abscissa_value (dim, value);

  if (dim == 0)
    poly.set_abscissa (value);
  else if (dim == 1)
    for (unsigned i=0; i < coefficients.size(); i++)
      coefficients[i].set_abscissa (value);
  else
    throw Error (InvalidParam, "Polynomial2D::set_abscissa",
                 "invalid dim=%u > 1", dim);
}

//! Set the abscissa offset
void MEAL::Polynomial2D::set_abscissa_offset (unsigned dim, double offset)
{
  if (dim == 0)
    poly.set_abscissa_offset (offset);
  else if (dim == 1)
    for (unsigned i=0; i < coefficients.size(); i++)
      coefficients[i].set_abscissa_offset (offset);
  else
    throw Error (InvalidParam, "Polynomial2D::set_abscissa",
                 "invalid dim=%u > 1", dim);
}

//! Copy constructor
MEAL::Polynomial2D::Polynomial2D (const Polynomial2D& copy)
: Multivariate<Scalar> (2)
{
  init ();
  operator = (copy);
}

//! Assignment operator
const MEAL::Polynomial2D&
MEAL::Polynomial2D::operator = (const Polynomial2D& copy)
{
  if (this == &copy)
    return *this;

  unsigned ncoeff = copy.poly.get_nparam();
  unsigned mcoeff = 0;
  if (copy.coefficients.size() > 0) 
    mcoeff = copy.coefficients[0].get_nparam();

  resize ( ncoeff, mcoeff );

  poly = copy.poly;
  for (unsigned i=0; i < coefficients.size(); i++)
    coefficients[i] = copy.coefficients[i];

  Multivariate<Scalar>::operator = (copy);
  return *this;
}
  
std::string MEAL::Polynomial2D::get_name() const
{
  return "Polynomial2D";
}

// void MEAL::Polynomial2D::parse (const string& line)
// {
//   Function::parse(line); // avoid using inherited GroupRule::parse()
// }

