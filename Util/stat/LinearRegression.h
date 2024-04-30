//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2024 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

// psrchive/Util/stat/LinearRegression.h

#ifndef __Util_stat_LinearRegression_h
#define __Util_stat_LinearRegression_h

#include "Estimate.h"
#include <vector>

class LinearRegression
{
public:

  //! If true, the weighted median difference between x and y is used instead of the weighted mean difference
  bool robust_offset = false;

  //! If true, the weighted mean of x is subracted from x, thereby reducing the covariance between scale and offset to zero
  bool subtract_weighted_mean_abscissa = false;

  //! Best-fit scale/slope and its formal uncertainty
  Estimate<double> scale;
  
  //! Best-fit offset/y-intercept and its formal uncertainty
  Estimate<double> offset;

  //! Covariance between scale and offset
  double covariance = 0.0;

  //! ordinary least squares with optional mask
  void ordinary_least_squares (const std::vector<double>& yval,
                               const std::vector<double>& xval,
                               const std::vector<bool>* mask = 0);

  //! weighted least squares
  /*! The weights are typically the inverse of the variance */
  void weighted_least_squares (const std::vector<double>& yval,
                               const std::vector<double>& xval,
                               const std::vector<double>& weight);

  //! generalized least squares
  /*! alpha contains the dot products of each basis vector and the all-ones vector.
      Typically, the ith element is the sum of the elements of the ith eigenvector.
      yval and xval are projected onto new basis
  */
  void generalized_least_squares (const std::vector<double>& yval,
                                  const std::vector<double>& xval,
                                  const std::vector<double>& weight,
                                  const std::vector<double>& alpha);
};

#endif
