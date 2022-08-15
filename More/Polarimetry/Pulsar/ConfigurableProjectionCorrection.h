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
#include "Pulsar/VariableTransformation.h"

#include "MEAL/Axis.h"

namespace Pulsar 
{
  //! Manager of variable transformations
  class ConfigurableProjectionCorrection : public VariableTransformationManager
  {
  public:

    //! Construct from a configuration file
    ConfigurableProjectionCorrection (const std::string& filename);

    // typedef MEAL::Axis< VariableTransformation::Argument > VariableArgument;

    class Transformation : public VariableTransformationManager::Transformation
    {
    protected:
      
      //! The variable transformation argument
      MEAL::Axis< Calibration::VariableTransformation::Argument > argument;

      //! The variable transformation
      Reference::To<Calibration::VariableTransformation> transformation;
      
    public:

      Transformation (Calibration::VariableTransformation* xform)
      {
        transformation = xform;
	argument.signal.connect (xform,
				 &Calibration::VariableTransformation::set_argument);
      }
      
      //! The transformation
      Calibration::VariableTransformation* get_transformation () 
      { return transformation; }
      
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

    //! Model inserted between instrument and projection
    /*! This attribute is cloned in each new Transformation */
    Reference::To< Calibration::VariableTransformation > transformation;

    //! Names of Archive attributes assigned to each abscissa/dimension
    std::map< unsigned, std::vector<std::string> > parameters;
  };
}

#endif
