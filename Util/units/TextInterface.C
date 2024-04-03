/***************************************************************************
 *
 *   Copyright (C) 2004-2009 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "TextInterface.h"
#include "NestedTextInterface.h"
#include "ModifyRestore.h"
#include "stringtok.h"
#include "pad.h"

#include <stdio.h>
#include <string.h>

#include <algorithm>

using namespace std;

bool TextInterface::label_elements = false;

// #define _DEBUG

bool TextInterface::matches (const string& name,
			     const string& prefix,
			     const Value* value)
{
#ifdef _DEBUG
  cerr << "TextInterface::matches prefix=" << prefix 
       << " name=" << name;
#endif

  string::size_type length = prefix.length();

  if (length)
  {
    if ( name.length() <= length || name[length] != ':' )
    {
#ifdef _DEBUG
      cerr << " false (no colon)" << endl;
#endif
      return false;
    }

    if ( strncmp (name.c_str(), prefix.c_str(), length) != 0 )
    {
#ifdef _DEBUG
      cerr << " false (prefix mismatch)" << endl;
#endif
      return false;
    }

    length ++;
  }

  string remainder = name.substr (length);

#ifdef _DEBUG
  cerr << "TextInterface::matches nested name="
       << value->get_name() << " remain=" << remainder << endl;
#endif

  bool result = value->matches (remainder);

#ifdef _DEBUG
  cerr << "TextInterface::matches result=" << result << endl;
#endif
  return result;
}


/*! Parses text into key, range, and remainder
  \retval true if key matches name
*/
bool TextInterface::match (const string& name, const string& text,
			   string* range, string* remainder)
{
#ifdef _DEBUG
  cerr << "TextInterface::match name=" << name << " text=" << text;
#endif

  string::size_type length = name.length();

  if ( length && strncmp (text.c_str(), name.c_str(), length) != 0 )
  {
#ifdef _DEBUG
    cerr << " false" << endl;
#endif
    return false;
  }

  string::size_type end = text.find (':', length);
  if (end == string::npos)
  {
    if (remainder)
    {
#ifdef _DEBUG
      cerr << " false" << endl; 
#endif
      return false;
    }
    else
      end = text.length();
  }

#ifdef _DEBUG
  cerr << " maybe" << endl;
#endif

  // the range is everything between the end of the variable name and the colon
  *range = text.substr (length, end-length);

#ifdef _DEBUG
  cerr << "TextInterface::match range=" << *range << endl;
#endif

  // the remainder is everything following the colon
  if (remainder)
  {
    *remainder = text.substr (end+1);
#ifdef _DEBUG
    cerr << "TextInterface::match remainder=" << *remainder << endl;
#endif
  }

  // a map may have no name
  if (!length)
    return true;

  // the range may be a wildcard
  if ((*range) == "*" || (*range) == "?")
    return true;

  length = range->length();

  // a range may be empty ...
  if (!length)
    return true;

  // .. or must be enclosed in square brackets
  if ((*range)[0] == '[' && (*range)[length-1] == ']')
    return true;

#ifdef _DEBUG
  cerr << "FALSE" << endl;
#endif

  return false;
}

