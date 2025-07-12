/***************************************************************************
 *
 *   Copyright (C) 2004-2025 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/DeFaradayed.h"
#include "Pulsar/DeFaraday.h"
#include "Pulsar/AuxColdPlasma.h"

using namespace std;

bool diff (double x, double y)
{
  return fabs(x-y)/(x+y) > 1e-15;
}

void Pulsar::DeFaradayed::apply (const Archive* archive)
{
  if (Archive::verbose > 2)
    cerr << "Pulsar::DeFaradayed::apply verification" << endl;

  unsigned nsubint = archive->get_nsubint();

  for (unsigned isubint=0; isubint < nsubint; isubint++)
  {
    check_relative (archive, isubint);
    check_absolute (archive, isubint);
  }
}

void Pulsar::DeFaradayed::check_relative (const Archive* archive, unsigned isubint)
{
  const Integration* subint = archive->get_Integration (isubint);
  const DeFaraday* ext = subint->get<DeFaraday>();

  if (!archive->get_faraday_corrected())
  {
    if (ext && ext->get_relative()->get_corrected())
    throw Error (InvalidState, "Pulsar::DeFaradayed::apply",
                "Archive::faraday_corrected is not set and Integration[%d]\n\t"
                "has a DeFaraday Extension with the relative correction flag set", isubint);
  }
  else
  {
    if (!ext)
        throw Error (InvalidState, "Pulsar::DeFaradayed::apply",
                "Archive::faraday_corrected is set and Integration[%d]\n\t"
                "has no DeFaraday Extension", isubint);

    if (!ext->get_relative()->get_corrected())
        throw Error (InvalidState, "Pulsar::DeFaradayed::apply",
                "Archive::faraday_corrected is set and Integration[%d]\n\t"
                "has a DeFaraday Extension without the relative correction flag set", isubint);

    if (diff( ext->get_relative()->get_reference_frequency(), archive->get_centre_frequency() ))
        throw Error (InvalidState, "Pulsar::DeFaradayed::apply",
                "Archive::faraday_corrected is set and Integration[%d]\n\t"
                "DeFaraday::reference_frequency = %lf doesn't equal\n\t"
                "Archive::centre_frequency = %lf", isubint,
                ext->get_relative()->get_reference_frequency(),
                archive->get_centre_frequency());

    if (diff( ext->get_relative()->get_measure(), archive->get_rotation_measure() ))
        throw Error (InvalidState, "Pulsar::DeFaradayed::apply",
                "Archive::faraday_corrected is set and Integration[%d]\n\t"
                "DeFaraday::get_relative()->measure = %lf does not equal\n\t"
                "Archive::rotation_measure = %lf", isubint,
                ext->get_relative()->get_measure(),
                archive->get_rotation_measure());
  }
}

void Pulsar::DeFaradayed::check_absolute (const Archive* archive, unsigned isubint)
{
  const Integration* subint = archive->get_Integration (isubint);
  const DeFaraday* ext = subint->get<DeFaraday>();

  const AuxColdPlasma* aux = archive->get<AuxColdPlasma>();

  if (!aux)
  {
    if (ext && ext->get_absolute()->get_corrected())
        throw Error (InvalidState, "Pulsar::DeFaradayed::apply",
                "Archive does not have an AuxColdPlasma extension and Integration[%d]\n\t"
                "has a DeFaraday Extension with the absolute correction flag set", isubint);
  }
  else if (aux->get_birefringence_corrected())
  {
    if (!ext)
        throw Error (InvalidState, "Pulsar::DeFaradayed::apply",
                "Archive has an AuxColdPlasma extension and Integration[%d]\n\t"
                "has no DeFaraday Extension", isubint);

    if (!ext->get_absolute()->get_corrected())
        throw Error (InvalidState, "Pulsar::DeFaradayed::apply",
                "Archive has an AuxColdPlasma extension and Integration[%d]\n\t"
                "has a DeFaraday Extension without the absolute correction flag set", isubint);

    if (diff( ext->get_absolute()->get_reference_frequency(), archive->get_centre_frequency() ))
        throw Error (InvalidState, "Pulsar::DeFaradayed::apply",
                "Archive has an AuxColdPlasma extension and Integration[%d]\n\t"
                "DeFaraday::reference_frequency = %lf doesn't equal\n\t"
                "Archive::centre_frequency = %lf", isubint,
                ext->get_absolute()->get_reference_frequency(),
                archive->get_centre_frequency());
  }
}
