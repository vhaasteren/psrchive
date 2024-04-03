//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2018 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

// psrchive/Base/Classes/Pulsar/StrategySet.h

#ifndef __Pulsar_StrategySet_h
#define __Pulsar_StrategySet_h

#include "Pulsar/ProfileStrategies.h"

namespace Pulsar {

  // forward declaration of ProfileStats class, which manages strategies
  class ProfileStats;

  class StrategySet : public Profile::Strategies
  {
    //! The default manager of Profile class strategies
    mutable Reference::To<ProfileStats> stats;

  public:

    //! The default implementation of the baseline finding algorithm
    /*! The default baseline estimator is the BaselineWindow class */
    static Configuration::Parameter< Reference::To<ProfileWeightFunction> >& 
    get_default_baseline();

    //! The default implementation of the onpulse finding algorithm
    /*! The default on-pulse estimator is the PeakConsecutive class */
    static Configuration::Parameter< Reference::To<ProfileWeightFunction> >& 
    get_default_onpulse();

    //! The default implementation of the S/N estimator
    /*! The default S/N estimator is the PhaseSNR class */
    static Configuration::Parameter< Reference::To<SNRatioEstimator> >& 
    get_default_snratio();

    //! Default constructor
    StrategySet ();

    //! Copy constructor
    StrategySet (const StrategySet&);

    //! Clone
    StrategySet* clone () const;

    //! The implementation of the baseline finding algorithm
    ProfileWeightFunction* baseline () const;
    void set_baseline (ProfileWeightFunction*);
    
    //! The implementation of the on-pulse finding algorithm
    ProfileWeightFunction* onpulse () const;
    void set_onpulse (ProfileWeightFunction*);

    //! The implementation of the signal-to-noise ratio calculation
    SNRatioEstimator* snratio () const;
    void set_snratio (SNRatioEstimator*);

    //! The implementation of the pulse width calculation
    WidthEstimator* width () const;
    void set_width (WidthEstimator*);

    //! The manager of Profile Strategies
    ProfileStats* get_stats () const;
    void set_stats (ProfileStats*);

    //! The Archive::Extension used to clone StrategySet during Archive::copy
    class Extension;
  };

}

#endif
