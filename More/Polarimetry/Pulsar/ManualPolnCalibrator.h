/***************************************************************************
 *
 *   Copyright (C) 2013 by Gregory Desvignes
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#ifndef __CalibrationManualPolnCalibrator_H
#define __CalibrationManualPolnCalibrator_H

#include "Pulsar/PolnCalibrator.h"
#include "Jones.h"

namespace Pulsar {

  //! Manages a table of Jones matrix elements as a function of time and frequency
  class ManualPolnCalibrator : public PolnCalibrator
  {

  public:

    //! Default Constructor
    ManualPolnCalibrator (const std::string ascii_model_filename);

    //! Copy Constructor
    ManualPolnCalibrator (const ManualPolnCalibrator& s);

    //! Destructor
    ~ManualPolnCalibrator ();

    virtual void load (const std::string& ascii_model_filename);

    // ///////////////////////////////////////////////////////////////////
    //
    // useful for calibrating
    //
    // ///////////////////////////////////////////////////////////////////

    //! Set the number of frequency channels in the response array
    virtual void set_response_nsub (unsigned nsub);

    //! Get the number of frequency channels in the response array
    virtual unsigned get_response_nsub () const;

    // ///////////////////////////////////////////////////////////////////
    //
    // Model implementation
    //
    // ///////////////////////////////////////////////////////////////////

    //! Calibrate the polarization of the given archive
    void calibrate (Archive* archive);

    //! The response at a single radio frequency
    class Response
    {
    public:

      //! load from ascii string and return the epoch
      MJD load (const std::string& str);

      //! set the reference frequency
      void set_frequency (double f) { frequency = f; }

      //! get the reference frequency
      double get_frequency () const { return frequency; }

      //! set the Jones matrix
      void set_response (const Jones<float>& J) { response = J; }

      //! get the Jones matrix
      const Jones<float>& get_response () const { return response; }

    protected:
 
      //! the reference frequency
      double frequency = 0.0; 

      //! the response funtion
      Jones<float> response;
    };

    //! The frequency response for a given epoch
    class FrequencyResponse
    {
    public:

      //! set the reference epoch
      void set_epoch (const MJD& mjd) { epoch = mjd; }

      //! get the reference eopch
      MJD get_epoch () const { return epoch; }

      //! get the Jones matrix for the specified frequency
      const Response& match (double freq_MHz) const;

      //! Add a new Response
      void add (const Response& single) { response.push_back(single); }

    protected:

      //! the reference epoch
      MJD epoch;

      //! the frequency response
      std::vector<Response> response;
    };

    //! Add a new Response at the given epoch
    void add(const MJD& epoch, const Response& single);

    //! Returns the best match, given the epoch
    const FrequencyResponse& match (const MJD& epoch) const;

    //! Returns the best match, given the epoch and frequency
    const Jones<float>& get_response (const MJD& epoch, double MHz) const;

  protected:

    //! The frequency response as a function of epoch
    std::vector<FrequencyResponse> response;

    //! Index of the current frequency response
    unsigned current_response = 0;

    //! Name of the file from which the entries were loaded
    std::string ascii_model_filename;
  };
}

#endif

