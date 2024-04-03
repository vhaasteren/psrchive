//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2018 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

// psrchive/Base/Classes/Pulsar/ProfileStrategies.h

#ifndef __Pulsar_ProfileStrategies_h
#define __Pulsar_ProfileStrategies_h

#include "Pulsar/Profile.h"

namespace Pulsar {

  class WidthEstimator;

  //! Manages the strategies that implement algorithms
  class Profile::Strategies : public Reference::Able
  {
  public:

    //! Destructor
    virtual ~Strategies () {}

    //! The implementation of the baseline finding algorithm
    virtual ProfileWeightFunction* baseline () const = 0;

    //! The implementation of the on-pulse finding algorithm
    virtual ProfileWeightFunction* onpulse () const = 0;

    //! The implementation of the signal-to-noise ratio calculation
    virtual SNRatioEstimator* snratio () const = 0;

    //! The implementation of the pulse width estimator
    virtual WidthEstimator* width () const = 0;

    //! Clone
    virtual Strategies* clone () const = 0;
  };

}

#endif
