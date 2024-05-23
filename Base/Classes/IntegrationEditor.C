/***************************************************************************
 *
 *   Copyright (C) 2010 - 2024 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/IntegrationEditor.h"
#include "Pulsar/IntegrationExtension.h"

using namespace std;

void Pulsar::IntegrationEditor::add_extension (Integration* subint, const std::string& ext)
{
  if (Integration::verbose > 1)
    cerr << "Pulsar::IntegrationEditor::add_extension " << ext << endl;

  subint->add_extension( Integration::Extension::factory( ext ) );
}

void Pulsar::IntegrationEditor::remove_extension (Integration* subint, const std::string& name)
{
  if (Integration::verbose > 1)
    cerr << "Pulsar::IntegrationEditor::remove_extension " << name << endl;

  unsigned next = subint->get_nextension();

  for (unsigned i=0; i<next; i++)
  {
    auto ext = subint->get_extension(i);
    if (ext->get_extension_name() == name || ext->get_short_name() == name)
      delete subint->get_extension(i);
  }
}



