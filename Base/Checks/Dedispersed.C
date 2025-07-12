/***************************************************************************
 *
 *   Copyright (C) 2004-2025 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/Dedispersed.h"
#include "Pulsar/Dedisperse.h"
#include "Pulsar/AuxColdPlasma.h"

using namespace std;

// defined in DeFaradayed.C
bool diff (double x, double y);

void Pulsar::Dedispersed::apply (const Archive* archive)
{
  if (Archive::verbose > 2)
    cerr << "Pulsar::Dedispersed::apply verification" << endl;

  unsigned nsubint = archive->get_nsubint();

  for (unsigned isubint=0; isubint < nsubint; isubint++)
  {
    check_relative (archive, isubint);
    check_absolute (archive, isubint);
  }
}

void Pulsar::Dedispersed::check_relative (const Archive* archive, unsigned isubint)
{
  const Integration* subint = archive->get_Integration (isubint);
  const Dedisperse* ext = subint->get<Dedisperse>();

  if (!archive->get_dedispersed())
  {
    if (ext && ext->relative.get_corrected())
    throw Error (InvalidState, "Pulsar::Dedispersed::apply",
                "Archive::dedispsersed is not set and Integration[%d]\n\t"
                "has a Dedisperse Extension with the relative correction flag set", isubint);
  }
  else
  {
    if (!ext)
        throw Error (InvalidState, "Pulsar::Dedispersed::apply",
                "Archive::dedispsersed is set and Integration[%d]\n\t"
                "has no Dedisperse Extension", isubint);

    if (!ext->relative.get_corrected())
        throw Error (InvalidState, "Pulsar::Dedispersed::apply",
                "Archive::dedispsersed is set and Integration[%d]\n\t"
                "has a Dedisperse Extension without the relative correction flag set", isubint);

    if (diff( ext->relative.get_reference_frequency(), archive->get_centre_frequency() ))
        throw Error (InvalidState, "Pulsar::Dedispersed::apply",
                "Archive::dedispsersed is set and Integration[%d]\n\t"
                "Dedisperse::reference_frequency = %lf doesn't equal\n\t"
                "Archive::centre_frequency = %lf", isubint,
                ext->relative.get_reference_frequency(),
                archive->get_centre_frequency());

    if (diff( ext->relative.get_measure(), archive->get_dispersion_measure() ))
        throw Error (InvalidState, "Pulsar::Dedispersed::apply",
                "Archive::dedispsersed is set and Integration[%d]\n\t"
                "Dedisperse::relative.measure = %lf does not equal\n\t"
                "Archive::dispersion_measure = %lf", isubint,
                ext->relative.get_measure(),
                archive->get_dispersion_measure());
  }
}

void Pulsar::Dedispersed::check_absolute (const Archive* archive, unsigned isubint)
{
  const Integration* subint = archive->get_Integration (isubint);
  const Dedisperse* ext = subint->get<Dedisperse>();

  const AuxColdPlasma* aux = archive->get<AuxColdPlasma>();

  if (!aux)
  {
    if (ext && ext->absolute.get_corrected())
        throw Error (InvalidState, "Pulsar::Dedispersed::apply",
                "Archive does not have an AuxColdPlasma extension and Integration[%d]\n\t"
                "has a Dedisperse Extension with the absolute correction flag set", isubint);
  }
  else
  {
    if (!ext)
        throw Error (InvalidState, "Pulsar::Dedispersed::apply",
                "Archive has an AuxColdPlasma extension and Integration[%d]\n\t"
                "has no Dedisperse Extension", isubint);

    if (!ext->absolute.get_corrected())
        throw Error (InvalidState, "Pulsar::Dedispersed::apply",
                "Archive has an AuxColdPlasma extension and Integration[%d]\n\t"
                "has a Dedisperse Extension without the absolute correction flag set", isubint);

    if (diff( ext->absolute.get_reference_frequency(), archive->get_centre_frequency() ))
        throw Error (InvalidState, "Pulsar::Dedispersed::apply",
                "Archive has an AuxColdPlasma extension and Integration[%d]\n\t"
                "Dedisperse::reference_frequency = %lf doesn't equal\n\t"
                "Archive::centre_frequency = %lf", isubint,
                ext->absolute.get_reference_frequency(),
                archive->get_centre_frequency());
  }
}
