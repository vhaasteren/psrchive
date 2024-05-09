//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2021 - 2024 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

// psrchive/Util/stat/ChiSquared.h

#ifndef __Util_stat_ChiSquared_h
#define __Util_stat_ChiSquared_h

#include "BinaryStatistic.h"

namespace BinaryStatistics
{
  class ChiSquared : public BinaryStatistic
  {
    bool robust_linear_fit;
    double outlier_threshold;
    double max_zap_fraction;

    std::vector<double> residual;
    
  public:

    //! Default constructor
    ChiSquared ();

    //! Set the threshold used to detect outliers during robust linear fit
    void set_outlier_threshold (double threshold) { outlier_threshold = threshold; }
    
    //! Return the chi-squared difference between 
    double get (const std::vector<double>&, const std::vector<double>&);

    //! Return the residual
    const std::vector<double>& get_residual () const { return residual; }
    
    ChiSquared* clone () const { return new ChiSquared; }
  };
}

#endif
