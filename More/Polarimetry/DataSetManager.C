/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include <Pulsar/DataSetManager.h>
#include <Pulsar/Archive.h>

using namespace Pulsar;
using namespace std;

DataSet::DataSet ()
{
  total_integration_length = 0;
}

//! Return true if data can be added to this data set
bool DataSet::matches (const Archive* archive)
{
  // empty, so anything matches
  if (data.size() == 0)
    return true;

  string reason;
  
  return data[0]->mixable(archive, reason);
}
  
//! Add data to this data set
void DataSet::add (const Archive* archive)
{
  if (!matches (archive))
    throw Error (InvalidParam, "DataSet::add", "archive does not match");

  name = archive->get_source();

  total_integration_length += archive->integration_length ();
  data.push_back (archive);
}
