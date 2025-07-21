/***************************************************************************
 *
 *   Copyright (C) 2007-2025 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Error.h"
#include "strutil.h"

#include <stdlib.h>
#include <libgen.h>

using namespace std;

string expand (const string& filename)
{
  if (filename[0] != '~')
    return filename;

  static const char* home = getenv("HOME");

  if (!home)
    throw Error (FailedSys, "expand(string&)", "Cannot determine $HOME");

  if (filename[1] == '/')
    return home + filename.substr(1);

  string system_home = pathname (home);
  return system_home + filename.substr(1);
}


// returns the directory name (using std C dirname)
std::string dirname (const std::string& path)
{
  std::string copy = path;
  return dirname( const_cast<char*>(copy.c_str()) );
}

// returns the file name (using std C basename)
std::string basename (const std::string& path)
{
  std::string copy = path;
  return basename( const_cast<char*>(copy.c_str()) );
}