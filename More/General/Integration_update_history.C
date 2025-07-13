/***************************************************************************
 *
 *   Copyright (C) 2025 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/Integration.h"
#include "Pulsar/AuxColdPlasmaMeasures.h"
#include "Pulsar/Dedisperse.h"
#include "Pulsar/DeFaraday.h"

using namespace std;

void Pulsar::Integration::update_history () try
{
  if (verbose)
    cerr << "Pulsar::Integration::update_history" << endl;

  auto aux = get<AuxColdPlasmaMeasures> ();

  if (!aux)
  {
    if (verbose)
        cerr << "Pulsar::Integration::update_history no AuxColdPlasmaMeasures" << endl;
    return;
  }

  {
    auto history = getadd<Dedisperse>();
    history->get_absolute()->set_corrected(true);
    history->get_absolute()->set_measure(aux->get_dispersion_measure());
    history->get_absolute()->set_reference_frequency(get_centre_frequency());
  }

  {
    auto history = getadd<DeFaraday>();
    history->get_absolute()->set_corrected(true);
    history->get_absolute()->set_measure(aux->get_rotation_measure());
    history->get_absolute()->set_reference_frequency(get_centre_frequency());
  }
}
catch (Error& error)
{
  throw error += "Pulsar::Integration::update_history";
}
