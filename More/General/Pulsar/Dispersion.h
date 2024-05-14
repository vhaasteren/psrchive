//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2006-2010 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

// psrchive/More/General/Pulsar/Dispersion.h

#ifndef __Pulsar_Dispersion_h
#define __Pulsar_Dispersion_h

#include "Pulsar/ColdPlasma.h"
#include "Pulsar/Config.h"
#include "Pulsar/DispersionDelay.h"
#include "Pulsar/Dedisperse.h"
#include "Pulsar/IntegrationBarycentre.h"

namespace Pulsar {

  //! Corrects dispersive delays
  /*! Rotates the phase of each profile in each frequency channel to
    remove dispersive delays with respect to the reference frequency.

    \post All profiles will be phase-aligned to the reference frequency
  */
  class Dispersion : public ColdPlasma<DispersionDelay,Dedisperse> {

  public:

    //! Default constructor
    Dispersion ();

    //! Return the dispersion measure due to the ISM
    double get_correction_measure (const Integration*) override;

    //! Return the auxiliary dispersion measure (0 if corrected)
    double get_absolute_measure (const Integration*) override;

    //! Return true if the Integration has been dedispersed
    bool get_corrected (const Integration* data) override;

    //! Return the effective dispersion measure that remains to be corrected
    double get_effective_measure (const Integration*) override;

    //! Return zero delay
    double get_identity () override { return 0; }

    //! Combine delays
    void combine (double& result, const double& add) override { result += add; }

    //! Setup all necessary attributes
    void update (const Integration*) override;

    //! Phase rotate each profile by the correction
    void apply (Integration*, unsigned channel) override;

    //! Apply the current correction to all sub-integrations in an archive
    void execute (Archive*) override;

    //! Undo the correction
    void revert (Archive*) override;

    //! Set the dispersion measure
    void set_dispersion_measure (double dispersion_measure)
    { set_measure (dispersion_measure); }
      
    //! Get the dispersion measure
    double get_dispersion_measure () const
    { return get_measure (); }

    //! Get the phase shift
    double get_shift () const;

  protected:

    double folding_period;

    static Option<bool> barycentric_correction;
    IntegrationBarycentre bary;
    double earth_doppler;

  };

}

#endif
