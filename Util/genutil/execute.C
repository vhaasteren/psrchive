/***************************************************************************
 *
 *   Copyright (C) 2023 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "execute.h"
#include "Error.h"
#include "tostring.h"
#include "stringtok.h"

#include <stdlib.h>

using namespace std;

// #define _DEBUG 1

#ifdef _DEBUG
#define DEBUG(x) cerr << x << endl;
#else
#define DEBUG(x)
#endif

string execute (const string& text, char cstart, char cend) try
{
  string remain = text;
    
  string::size_type start;
  string::size_type end;

  DEBUG("execute text='" << text << "'");

  while ( (end = remain.find(cend)) != string::npos &&
	  (start = remain.rfind(cstart, end)) != string::npos && end != start)
  {
    // find the end of the variable name
    string eval = remain.substr (start+1, end-start-1);

    DEBUG("eval='" << eval << "'");

    // string preceding the variable substitution
    string before = remain.substr (0, start);

    DEBUG("before='" << before << "'");

    end ++;

    // string following the variable substitution
    string after = (end != string::npos) ? remain.substr (end) : "";

    DEBUG("after='" << after << "'");

    string subst;

    int retval = system (eval.c_str());
    if (retval == -1)
      throw Error (FailedCall, "system("+eval+")");

    subst = tostring(WIFEXITED(retval));

    remain = before + subst + after;
  }

  DEBUG("remain='" << remain << "'");

  return remain;
}
catch (std::exception& error)
{
  throw Error (InvalidParam, "execute", error.what());
}
