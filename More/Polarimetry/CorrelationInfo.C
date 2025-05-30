/***************************************************************************
 *
 *   Copyright (C) 2025 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/CorrelationInfo.h"
#include "Pulsar/PolnCalibratorInfo.h"

using namespace Pulsar;
using namespace std;

CorrelationInfo::CorrelationInfo (unsigned idx, const PolnCalibrator* cal) 
{
  index = idx;
  poln_calibrator = cal;

  auto cal_info = cal->get_Info();

  if (!cal_info)
    return;

  auto info = dynamic_cast<PolnCalibrator::Info*>(cal_info);

  if (!info)
    return;

  unsigned nclass = info->get_nclass();
  for (unsigned iclass=0; iclass < nclass; iclass++)
  {
    unsigned nparam = info->get_nparam(iclass);
    for (unsigned iparam=0; iparam < nparam; iparam++)
    {
      names.push_back( info->get_param_name(iclass, iparam) );
    }
  }
}

string CorrelationInfo::get_title () const
{
  if (index < names.size())
    return "Cross-correlation between " + names[index] + "\\fn and other parameters";
  else
    return "Cross-correlation between parameters";
}

unsigned CorrelationInfo::get_nchan () const
{
  return poln_calibrator->get_nchan();
}

constexpr unsigned backend_param = 3;

//! Return the number of parameter classes
unsigned CorrelationInfo::get_nclass () const
{
  if (names.size() > backend_param)
    return 2;
  else
    return 1;
}

std::string CorrelationInfo::get_param_name (unsigned iclass, unsigned iparam) const
{
  if (iclass == 1)
    iparam += backend_param;

  return names[iparam];
} 

//! Return the name of the specified class
string CorrelationInfo::get_label (unsigned iclass) const
{
  unsigned start_param = 0;
  unsigned nparam = names.size();

  if (iclass == 0)
  {
    nparam = std::min(nparam, backend_param);
  }
  else
  {
    start_param = backend_param;
  }

  if (start_param == index)
    start_param ++;

  string retval = names[start_param];
  for (unsigned i=start_param+1; i<nparam; i++)
  {
    if (i != index)
      retval += " " + names[i];
  }

  return retval;
}
  
//! Return the number of parameters in the specified class
unsigned CorrelationInfo::get_nparam (unsigned iclass) const
{
  unsigned nparam = 0;

  if (iclass == 0)
  {
    nparam = backend_param;
    if (index < backend_param)
      nparam --;
  }
  else
  {
    nparam = names.size();
    if (index >= backend_param)
      nparam --;
  }

  return nparam;
}

Estimate<float> CorrelationInfo::get_param (unsigned ichan, unsigned iclass, unsigned iparam) const
{
  if (!poln_calibrator->get_transformation_valid(ichan))
    return 0.0;

  if (!poln_calibrator->has_covariance())
    return 0.0;

  std::vector<double> covar;
  poln_calibrator->get_covariance (ichan, covar);

  unsigned nparam = names.size();
  unsigned expected_size = nparam * (nparam + 1) / 2;

  if (covar.size() != expected_size)
    return 0.0;

  return Estimate<float>(1,1);
}
