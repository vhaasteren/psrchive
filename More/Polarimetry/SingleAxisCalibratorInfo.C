/***************************************************************************
 *
 *   Copyright (C) 2003 - 2023 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/SingleAxisCalibratorInfo.h"

using namespace Pulsar;
using namespace std;

SingleAxisCalibrator::Info::Info (const PolnCalibrator* cal) 
  : PolnCalibrator::Info (cal)
{
}

string SingleAxisCalibrator::Info::get_title () const
{
  return "Polar Decomposition of Complex Gains";
}

//! Return the number of parameter classes
unsigned SingleAxisCalibrator::Info::get_nclass () const
{
  return 3; 
}

//! Return the name of the specified class
string SingleAxisCalibrator::Info::get_name (unsigned iclass) const
{
  switch (iclass) {
  case 0:
    return "\\fiG\\fr (\\fic\\fr\\d0\\u)";
  case 1:
    return "\\gg (%)";
  case 2:
    return "\\gf (\\(2729))";
  default:
    return "";
  }
} 
  
//! Return the number of parameters in the specified class
unsigned SingleAxisCalibrator::Info::get_nparam (unsigned iclass) const
{
  if (iclass < 3)
    return 1;
  return 0;
}

Estimate<float> SingleAxisCalibrator::Info::get_param (unsigned ichan, 
					       unsigned iclass,
					       unsigned iparam) const
{
  if (iclass == 0)
    return PolnCalibrator::Info::get_param (ichan, iclass, iparam);

  /*
    See Equation (25) of van Straten (2006) and the text that follows it.

    The SingleAxis model is parameterized by beta and this object provides gamma.
  */
  
  if (iclass == 1)
    return 100.0 * 
      ( exp( 2*PolnCalibrator::Info::get_param (ichan, iclass, iparam) ) - 1 );

  // iclass == 2
  return 180.0/M_PI * PolnCalibrator::Info::get_param (ichan, iclass, iparam);
}

Calibrator::Info* SingleAxisCalibrator::get_Info () const
{
  return new SingleAxisCalibrator::Info (this);
}

