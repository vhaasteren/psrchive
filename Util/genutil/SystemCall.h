//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2012 - 2023 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

/* Util/genutil/SystemCall.h */

#ifndef __Util_genutil_SystemCall_h
#define __Util_genutil_SystemCall_h

#include <string>

//! Executes a system call with multiple retries
class SystemCall
{
  unsigned retries {3};
  bool throw_exception {true};
  int return_value {0};

 public:

  //! Set the number of retries
  void set_retries (unsigned n) { retries = n; }

  //! Throw an exception if the command returns a non-zero exit status
  void set_throw (bool flag) { throw_exception = flag; }

  //! Get the return value (exit status) of the last executed command
  int get_return_value () const { return return_value; }
  
  //! Run the given command
  /*! If the command returns a non-zero exit status and throw_execption is true, then an exception is thrown. */
  void run (const std::string&);

};

#endif // !defined(__Util_genutil_SystemCall_h)

