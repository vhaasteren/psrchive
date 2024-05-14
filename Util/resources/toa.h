//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 1999 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

// psrchive/Util/tempo/toa.h

#ifndef __TOA_H
#define __TOA_H

#include <vector>
#include <string>

#include "residual.h"
#include "MJD.h"
#include "Estimate.h"

namespace Tempo {

  class toaInfo;

  // //////////////////////////////////////////////////////////////////////////
  //
  // toa - class that encapsulates TEMPO data format
  //
  // //////////////////////////////////////////////////////////////////////////

  //! Describes a pulse Time of Arrival (TOA)
  class toa
  {
    
  public:
    
    static const float UNSET;
    
    //! TOA output format
    enum Format
    { 
      Unspecified,
      Comment, 
      Princeton, 
      Parkes, 
      ITOA, 
      Psrclock, 
      Command,
      Tempo2
    };
    
    enum State
    { 
      Undefined = -2,  // point will never be plotted or used
      Deleted = -1,    // point will not be plotted unless an undelete happens
      Hidden = 0,      // point is temporarily outside of viewing region
      Normal = 1,      // point is in viewing region and plotted
      Selected = 2     // like Normal, but highlighted selection
    };
    
    static bool verbose;

  protected:
    
    // Fundamental TOA LINE as described here:
    // http://pulsar.princeton.edu/tempo/ref_man_sections/toa.txt

    //! Observing frequency (MHz)
    double frequency = 0.0;

    //! Time of arrival, TOA
    MJD arrival;

    //! TOA uncertainty in microseconds
    float error = 0.0;

    //! Goodness-of-fit (reduced chisq) reported by the phase shift estimator
    float reduced_chisq = 0.0;

    //! Phase shift (turns) computed by the phase shift estimator
    double phase_shift = 0;

    //! Observatory code
    std::string telescope;

    //! Frequency channel index in data file
    unsigned channel = 0;

    //! Sub-integration index in data file
    unsigned subint = 0;

    //! Phase offset (turns, added to TOA)
    /*! Parkes format-specific */
    float phs = 0;

    //! Observatory (two-letter code)
    char observatory[2];
    
    //! Typically the name of the data file
    std::string auxinfo;

    //! Signal-to-noise ratio estimate
    float signal_to_noise = 0;

    //! Parallactic angle ???
    float parallactic_angle = 0;

    //! Bandwidth (MHz)
    float bandwidth = 0;

    //! Duration (s) aka integration length
    float duration = 0;

    //! Dispersion measure, DM (pc cm^-3)
    double dispersion_measure = 0;

    //! Dispersion measure correction (pc cm^-3)
    /*! Parsed from Princeton format */
    float dispersion_measure_correction;

    //! Estimated dispersion measure (pc cm^-3)
    Estimate<double> dispersion_measure_estimate;

    Estimate<float> flux;

    bool phase_info;

    // one of the available formats on loading

    Format format;
    State state;

  public:

    // residual for this toa as calculated by tempo
    residual resid;
    
    // Basic constructors and destructors
    toa (Format fmt = Psrclock);
    virtual ~toa () { destroy(); };
    
    // copy constructor
    toa (const toa & in_toa);
    toa& operator = (const toa & in_toa);
    
    // construct from an open file
    toa (FILE* instream);
    
    // construct from a string
    toa (char* indata);
    
    // methods for setting/getting things (may eventually check validity)
    void set_format    (Format fmt)  { format = fmt; };
    void set_StoN      (float sn)    { signal_to_noise = sn; };
    void set_pa        (float p)     { parallactic_angle = p; };
    void set_bw        (float b)     { bandwidth = b; };
    void set_dur       (float d)     { duration = d; };
    void set_dm        (float d)     { dispersion_measure = d; };
    void set_state     (State st)    { state = st; };

    void set_frequency (double freq) { frequency = freq; };
    void set_arrival   (const MJD& arrived) { arrival = arrived; };
    void set_error     (float err)   { error = err; };
    void set_reduced_chisq (float x) { reduced_chisq = x; }
    void set_flux      (const Estimate<float>& x) { flux = x; }
    void set_dispersion_measure_estimate (const Estimate<double>& x) { dispersion_measure_estimate = x; }
    void set_telescope (const std::string& telcode);
    void set_auxilliary_text (const std::string& text) { auxinfo = text; };
    void set_channel   (unsigned chan)   { channel = chan; };
    void set_subint    (unsigned sub)    { subint = sub; };
    void set_phase_shift (double shift) { phase_shift = shift; };
    void set_phase_info  (bool info)    { phase_info = info; };

    Format get_format    () const { return format; };
    float  get_StoN      () const { return signal_to_noise; };
    float  get_pa        () const { return parallactic_angle; };
    float  get_bw        () const { return bandwidth; };
    float  get_dur       () const { return duration; };
    float  get_dm        () const { return dispersion_measure; };
    State  get_state     () const { return state; };

    double get_frequency () const { return frequency; };
    MJD    get_arrival   () const { return arrival; };
    float  get_error     () const { return error; };
    float  get_reduced_chisq () const { return reduced_chisq; }
    Estimate<float> get_flux () const { return flux; }
    Estimate<double> get_dispersion_measure_estimate() const { return dispersion_measure_estimate; }

    std::string get_telescope () const { return telescope; };
    std::string get_auxilliary_text () const { return auxinfo; };
    double get_phase_shift () const { return phase_shift; };
    unsigned get_channel     () const { return channel; };
    unsigned get_subint      () const { return subint; };


    // loading and unloading to/from file and string
    int    load   (FILE* instream);
    int    load   (const char* instring);
    int    unload (FILE* outstream, Format fmt = Unspecified) const;
    int    unload (char* outstring, Format fmt = Unspecified) const;
    
    int    parkes_parse     (const char* instring);
    int    parkes_out       (char* outstring) const;
    int    Parkes_load      (const char* instring);
    int    Parkes_unload    (FILE* outstream) const;
    int    Parkes_unload    (char* outstring) const;
    
    int    Princeton_load   (const char* instring);
    int    Princeton_unload (FILE* outstream) const;
    int    Princeton_unload (char* outstring) const;

    int    Psrclock_load    (const char* instring);
    int    Psrclock_unload  (FILE* outstream) const;
    int    Psrclock_unload  (char* outstring) const;

    std::string Psrclock_unload  () const;

    int    Command_load      (const char* instring);
    int    Command_unload    (FILE* outstream) const;
    int    Command_unload    (char* outstring) const;

    int    Comment_unload    (FILE* outstream) const;
    int    Comment_unload    (char* outstring) const;

	// -----------

    int    Tempo2_unload     (FILE* outstream) const;
    int    Tempo2_unload     (char* outstring) const;

    int    Tempo_unload      (FILE* outstream) const;
    int    Tempo_unload      (char* outstring) const;
    
    // comparison operators
    friend int operator < (const toa& t1, const toa& t2)
    { return (t1.arrival < t2.arrival); };
    
    // operations on vectors of toas
    static int load (const char* filename, std::vector<toa>* toas);
    static int load (FILE* instream, std::vector<toa>* toas);
    
    static int unload (const char* filename, const std::vector<toa>& toas,
		       Format fmt = Unspecified);
    static int unload (FILE* outstream, const std::vector<toa>& toas,
		       Format fmt = Unspecified);
    
  private:
    // low-level stuff
    void init();
    void destroy();
    bool valid();
    
    static void sizebuf (size_t length);
    static char*  buffer;
    static size_t bufsz;
    static char   datestr [25];
  };

  std::ostream& operator << (std::ostream&, toa::Format format);
  std::istream& operator >> (std::istream&, toa::Format& format);

}

#endif



