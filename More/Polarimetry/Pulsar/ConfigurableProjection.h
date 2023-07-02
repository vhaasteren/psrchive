//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#ifndef __Pulsar_ConfigurableProjection_h
#define __Pulsar_ConfigurableProjection_h

#include "Pulsar/VariableProjectionCorrection.h"
#include "Pulsar/VariableTransformation.h"

#include "Pulsar/ConfigurableProjectionExtension.h"

#include "MEAL/Axis.h"

namespace Pulsar 
{
  //! Manager of variable transformations
  class ConfigurableProjection : public VariableTransformationManager
  {
  public:

    //! Construct from a configuration file
    ConfigurableProjection (const std::string& filename);

    //! Construct from a ConfigurableProjectionExtension
    ConfigurableProjection (const ConfigurableProjectionExtension*);

    //! Construct from a configuration string
    void construct (const std::string&);

    //! Set the Archive for which a tranformation will be computed
    void set_archive (const Archive* _archive);

    //! Set the sub-integration for which a tranformation will be computed
    void set_subint (unsigned _subint);

    //! Set the frequency channel for which a tranformation will be computed
    void set_chan (unsigned _chan);

    class Transformation;

    //! Set the number of frequency channels
    void set_nchan (unsigned);

    //! Get the number of frequency channels
    unsigned get_nchan () const;

    //! Return a newly constructed Argument::Value for the given Transformation
    MEAL::Argument::Value* new_value (VariableTransformationManager::Transformation*);

    //! Return the value associated with the parameter name
    double get_value (const std::string& name);

    //! Get the configuration text from which this instance was constructed
    const std::string& get_configuration() const { return configuration; }

    //! Communicates parameters to plotting routines
    class Info;

    //! Calibrate an observation
    /*! \pre The backend should be fully corrected and calibrated, and the basis should be corrected */
    void calibrate (Archive*);

  protected:

    //! Configuration string
    std::string configuration;

    //! Known/fixed projection correction
    VariableProjectionCorrection projection;

    //! Model inserted between instrument and projection
    /*! This attribute is cloned in each new Transformation */
    Reference::To< Calibration::VariableTransformation > transformation;

    //! Names of Archive attributes assigned to each abscissa/dimension
    std::map< unsigned, std::vector<std::string> > parameters;

    std::vector< Reference::To<Transformation> > xforms;

  public:

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
        argument.signal.connect (xform, &Calibration::VariableTransformation::set_argument);
      }
  
      //! The transformation
      Calibration::VariableTransformation* get_transformation ()
      { return transformation; }
 
      //! The transformation
      const Calibration::VariableTransformation* get_transformation () const
      { return transformation; }
 
      //! Its argument
      MEAL::Argument* get_argument () { return &argument; }
    };

    //! Return the Transformation instance for the specified channel
    Transformation* get_transformation (unsigned ichan);

    //! Return the Transformation instance for the specified channel
    const Transformation* get_transformation (unsigned ichan) const;

    //! Return true if the speficied channel has a valid solution
    bool get_transformation_valid (unsigned ichan) const;

  };

  void copy (MEAL::Complex2* to,
             const ConfigurableProjectionExtension::Transformation* from);

  void copy (ConfigurableProjectionExtension::Transformation* to,
             const MEAL::Complex2* from);

}

#endif
