//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2023 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/
// psrchive/Util/genutil/evaluate.h

#ifndef __UTILS_GENUTIL_EXECUTE_H
#define __UTILS_GENUTIL_EXECUTE_H

#include <string>

//! Replaces strings enclosed in backticks ` ` with the return value of the system call
std::string execute (const std::string& text, char cstart='`', char cend='`');

#endif
