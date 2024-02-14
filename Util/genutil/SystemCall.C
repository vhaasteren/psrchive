/***************************************************************************
 *
 *   Copyright (C) 2012 - 2023 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "SystemCall.h"
#include "Error.h"
#include "fsleep.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

void SystemCall::run (const string& command)
{
  string errstr;

  if (!system(NULL))
    throw Error (InvalidState, "SystemCall::run", "shell not available; insufficient resources");
 
  bool must_throw = false;

  for (unsigned i=0; i < retries; i++)
  {    
    int err = system (command.c_str());

    if (WIFEXITED(err))
    {
      return_value = WEXITSTATUS(err);

      if (return_value == 0)
        return;
    }

    must_throw = true;

    // else an error occured
    if (err < 0)
    {
      errstr = strerror (err);
    }

    else if (WIFSIGNALED(err))
    {
      errstr = "\n\t" "terminated by signal " + string(strsignal(WTERMSIG(err)));
      if (WCOREDUMP(err))
        errstr += " (core dumped)";
    }

    else if (WIFSTOPPED(err))
    {
      errstr = "\n\t" "stopped by signal " + string(strsignal(WSTOPSIG(err)));
    }

    else if (return_value != 0)
    {
      must_throw = false;
      errstr = "\n\t" "returned error code " + tostring(return_value);
    }

    else
    {
      errstr = "\n\t" "failed for an unknown reason";
    }

    fsleep (5e-4);
    retries --; 
  }

  // the above loop finished without a successful return
  if (throw_exception || must_throw)
    throw Error (FailedCall, "SystemCall::run", "system (\"" + command + "\") failed: " + errstr);
}
