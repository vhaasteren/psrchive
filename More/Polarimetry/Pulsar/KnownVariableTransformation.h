//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#ifndef __Pulsar_KnownVariableTransformation_h
#define __Pulsar_KnownVariableTransformation_h

#include "Pulsar/VariableTransformationManager.h"
#include "MEAL/Axis.h"
#include "MEAL/Value.h"

namespace Pulsar {

  //! A Jones matrix with a label
  template<typename T>
  class LabelledJones : public Jones<T>
  {
    public:
      std::string label;

      LabelledJones() {}

      template<typename U>
      LabelledJones(const Jones<U>& jones) : Jones<T>(jones) {}
  };

  //! Specialize the template defined in MEAL/Axis.h
  template<typename T>
  std::string axis_value_to_string(const LabelledJones<T>& jones) { return jones.label; }

  //! Manager of variable transformations
  class KnownVariableTransformation : public VariableTransformationManager
  {
  public:


    typedef MEAL::Axis< LabelledJones<double> >   KnownArgument;
    typedef MEAL::Value< MEAL::Complex2 > KnownTransformation;

    class Transformation : public VariableTransformationManager::Transformation
    {
    protected:
      
      //! The known transformation argument
      KnownArgument argument;
      //! The known transformation
      KnownTransformation transformation;
      
    public:

      Transformation ()
      {
        argument.signal.connect (&transformation, &KnownTransformation::set_value);
      }
      
      //! The transformation
      MEAL::Complex2* get_transformation () { return &transformation; }
      
      //! Its argument
      MEAL::Argument* get_argument () { return &argument; }  
    };
    
    //! Return a newly constructed Transformation instance
    Transformation* get_transformation (unsigned ichan);

    //! Return a newly constructed Argument::Value for the given Transformation
    MEAL::Argument::Value* new_value (VariableTransformationManager::Transformation*);

    //! Derived classes define the known transformation
    virtual LabelledJones<double> get_transformation () = 0;
    
  };
}

#endif
