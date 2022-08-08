/***************************************************************************
 *
 *   Copyright (C) 2009 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "MEAL/FunctionInterface.h"
#include <algorithm>

using namespace std;

namespace MEAL
{
  //! Provides name-based access to parameter attributes in Function::Interface
  class Parameter : public Reference::Able
  {
    Reference::To<ParameterPolicy, false> parameter_policy;
    unsigned index;

    public:

    Parameter (ParameterPolicy* policy, unsigned i)
    { parameter_policy = policy, index = i; }

    // Text interface to a Parameter instance
    class Interface : public TextInterface::To<Parameter>
    {
      public:
          Interface( Parameter *s_instance = NULL );
    };

    //! Description of the parameter
    std::string get_description () const
    { return parameter_policy->get_description (index); }

    //! Return the value of the specified parameter
    double get_param () const
    { return parameter_policy->get_param (index); }

    //! Set the value of the specified parameter
    void set_param (double value)
    { parameter_policy->set_param (index,value); }

    //! Return the variance of the specified parameter
    double get_variance () const
    { return parameter_policy->get_variance (index); }

    //! Set the variance of the specified parameter
    void set_variance (double value)
    { parameter_policy->set_variance (index,value); }

    //! Return true if parameter at index is to be fitted
    bool get_infit () const
    { return parameter_policy->get_infit (index); }

    //! Set flag for parameter at index to be fitted
    void set_infit (bool flag)
    { parameter_policy->set_infit (index,flag); }
  };
}


MEAL::Parameter::Interface::Interface ( Parameter *s_instance )
{
  std::string value_name = "Parameter value";

  if ( s_instance )
  {
    set_instance( s_instance );
    string help = s_instance->get_description();
    if ( help != "" )
      value_name = help;
  }

  add( &Parameter::get_param,
       &Parameter::set_param,
       "val", value_name.c_str() );

  add( &Parameter::get_variance,
       &Parameter::set_variance,
       "var", "Parameter variance" );

  add( &Parameter::get_infit,
       &Parameter::set_infit,
       "fit", "Fit flag" );
}

MEAL::Function::Interface::Interface( Function *c )
{
  if (c)
  {
    name = c->get_name();
    description = c->get_description();

    set_instance (c);
  }

  add( &Function::get_nparam, "nparam", "Number of parameters" );

  {
    VGenerator<string> generator;

    add_value(generator( "name", string("Parameter name"),
			  &Function::get_param_name,
			  &Function::get_nparam ));

    add_value(generator( "help", string("Parameter description"),
			  &Function::get_param_description,
			  &Function::get_nparam ));
  }

  {
    VGenerator<double> generator;

    add_value(generator( "val", string("Parameter value"),
			  &Function::get_param,
			  &Function::set_param,
			  &Function::get_nparam ));

    add_value(generator( "var", string("Parameter variance"),
			  &Function::get_variance,
			  &Function::set_variance,
			  &Function::get_nparam ));
  }

  {
    VGenerator<bool> generator;

    add_value(generator( "fit", string("Fit flag"),
			  &Function::get_infit,
			  &Function::set_infit,
			  &Function::get_nparam ));
  }

  if (!c)
    return;

  if (!c->has_parameter_policy())
    return;

  ParameterPolicy* policy = c->get_parameter_policy();

  unsigned nparam = c->get_nparam();

  // check for duplicate names

  vector<string> names (nparam);
  for (unsigned iparam=0; iparam < nparam; iparam++)
    names[iparam] = c->get_param_name (iparam);

  std::sort (names.begin(), names.end());
  if (std::adjacent_find(names.begin(), names.end()) != names.end())
    return;

  // there are no duplicate names

  for (unsigned iparam=0; iparam < nparam; iparam++)
  {
    string name = c->get_param_name (iparam);
    Parameter* parameter = new Parameter (policy, iparam);
    insert( name, new Parameter::Interface (parameter) );
  }

#if 0
  add( &Function::get_,
       &Function::set_,
       "", "" );
#endif

}

