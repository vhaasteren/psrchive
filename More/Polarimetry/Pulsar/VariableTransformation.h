//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

// psrchive/More/Polarimetry/Pulsar/VariableTransformation.h

#ifndef __CalibrationVariableTransformation_H
#define __CalibrationVariableTransformation_H

#include "MEAL/Nvariate.h"
#include "MEAL/ProductRule.h"
#include "MEAL/ChainRule.h"
#include "MEAL/Value.h"
#include "MEAL/Complex2.h"

#include "Estimate.h"
#include "Stokes.h"

#include <map>

namespace Calibration {

  //! Product of multi-variate model and known correction

  class VariableTransformation : public MEAL::ProductRule<MEAL::Complex2>
  {
    //! Chain rule used to constrain model parameters with functions
    MEAL::ChainRule<MEAL::Complex2> chain;

    //! Any known correction
    MEAL::Value<MEAL::Complex2> correction;

    //! The model to be constrained
    Reference::To<MEAL::Complex2> model;

    //! Map of model index to constraining multivariate function
    std::map< unsigned, Reference::To< MEAL::Nvariate<MEAL::Scalar> > > function;

  public:

    //! Default Constructor
    VariableTransformation ();

    //! Copy Constructor
    VariableTransformation (const VariableTransformation& s);

    //! Assignment Operator
    const VariableTransformation& operator = (const VariableTransformation& s);

    //! Destructor
    ~VariableTransformation ();

    //! Clone operator
    VariableTransformation* clone () const;

    //! Set the model that is constrained by abscissae
    void set_model (MEAL::Complex2*);

    //! Get the model that is constrained by abscissae
    const MEAL::Complex2* get_model () const;
    MEAL::Complex2* get_model ();

    //! Set the multivariate function that constrains the specified parameter
    void set_constraint (unsigned index, MEAL::Nvariate<MEAL::Scalar>*);

    //! Get the number of constrained parameters
    unsigned get_nconstraint () const { return function.size(); }

    //! Return true if the index is constrained
    bool has_constraint (unsigned index) { return function.find(index) != function.end(); }

    class Argument
    {
    public:

      //! the known projection correction
      Jones<double> correction;

      //! the arguments for each abscissa
      std::map< unsigned, std::vector<double> > arguments;
    };

    void set_argument (const Argument&);

  private:

    //! Initialize function used by constructors
    void init ();

  };
}

#endif

