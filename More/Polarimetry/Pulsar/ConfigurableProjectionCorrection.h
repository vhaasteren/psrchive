//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#ifndef __Pulsar_ConfigurableProjectionCorrection_h
#define __Pulsar_ConfigurableProjectionCorrection_h

#include "Pulsar/VariableProjectionCorrection.h"

#include "MEAL/Multivariate.h"
#include "MEAL/ProductRule.h"
#include "MEAL/ChainRule.h"
#include "MEAL/Axis.h"

namespace Pulsar {

  //! Manager of variable transformations
  class ConfigurableProjectionCorrection : public VariableTransformationManager
  {
  public:

    //! Construct from a configuration file
    ConfigurableProjectionCorrection (const std::string& filename);

    class Argument
    {
    public:

      //! the known projection correction
      Jones<double> correction;

      //! the arguments for each abscissa
      std::vector<double> arguments;
    };

    typedef MEAL::Axis< Argument > ConfigurableArgument;

    class ConfigurableTransformation : public MEAL::ProductRule<MEAL::Complex2>
    {
    protected:

      std::map< unsigned, Reference::To< MEAL::Multivariate<MEAL::Scalar> > > function;

      //! the known projection correction
      MEAL::Value<MEAL::Complex2> correction;

      //! the channel index
      unsigned ichan;
      
    public:
      ConfigurableTransformation ();
      ~ConfigurableTransformation ();
      void set_argument (const Argument&);
    };

    class Transformation : public VariableTransformationManager::Transformation
    {
    protected:
      
      //! The transformation argument
      ConfigurableArgument argument;
      //! The transformation
      ConfigurableTransformation transformation;
      
    public:

      Transformation ()
      {
	argument.signal.connect (&transformation,
				 &ConfigurableTransformation::set_argument);
      }
      
      //! The transformation
      MEAL::Complex2* get_transformation () { return &transformation; }
      
      //! Its argument
      MEAL::Argument* get_argument () { return &argument; }  
    };

    //! Set the Archive for which a tranformation will be computed
    void set_archive (const Archive* _archive);

    //! Set the sub-integration for which a tranformation will be computed
    void set_subint (unsigned _subint);

    //! Set the frequency channel for which a tranformation will be computed
    void set_chan (unsigned _chan);

    //! Return a newly constructed Transformation instance
    Transformation* new_transformation ();

    //! Return a newly constructed Argument::Value for the given Transformation
    MEAL::Argument::Value* new_value (VariableTransformationManager::Transformation*);

    //! Return the value associated with the parameter name
    double get_value (const std::string& name);

  protected:

    //! Known/fixed projection correction
    VariableProjectionCorrection projection;

    //! Names of parameters assigned to each abscissa
    std::vector<std::string> parameters;
    
  };
}

#endif
