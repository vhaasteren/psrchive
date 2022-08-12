//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#ifndef __Pulsar_NancayProjectionCorrection_h
#define __Pulsar_NancayProjectionCorrection_h

#include "Pulsar/VariableProjectionCorrection.h"

#include "MEAL/Polynomial2D.h"
#include "MEAL/ProductRule.h"
#include "MEAL/Axis.h"

namespace Pulsar {

  //! Manager of variable transformations
  class NancayProjectionCorrection : public VariableTransformationManager
  {
  public:

    class Argument
    {
    public:
      //! the known projection correction
      Jones<double> correction;
      double declination;
      double hour_angle;
    };

    typedef MEAL::Axis< Argument > NancayArgument;

    class NancayTransformation : public MEAL::ProductRule<MEAL::Complex2>
    {
    protected:

      //! Variation of differential phase with hour angle and declination
      MEAL::Polynomial2D diff_phase;

      //! Variation of differential gain with hour angle and declination
      MEAL::Polynomial2D diff_gain;
      
      //! the known projection correction
      MEAL::Value<MEAL::Complex2> correction;

      //! the channel index
      unsigned ichan;
      
    public:
      NancayTransformation ();
      ~NancayTransformation ();
      void set_argument (const Argument&);
    };

    class Transformation : public VariableTransformationManager::Transformation
    {
    protected:
      
      //! The transformation argument
      NancayArgument argument;
      //! The transformation
      NancayTransformation transformation;
      
    public:

      Transformation ()
      {
	argument.signal.connect (&transformation,
				 &NancayTransformation::set_argument);
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


  protected:

    //! Known/fixed projection correction
    VariableProjectionCorrection projection;
    
  };
}

#endif
