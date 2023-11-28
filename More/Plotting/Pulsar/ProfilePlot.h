//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2006 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

// psrchive/More/Plotting/Pulsar/ProfilePlot.h

#ifndef __Pulsar_ProfilePlot_h
#define __Pulsar_ProfilePlot_h

#include "Pulsar/FluxPlot.h"
#include "Pulsar/HasPen.h"
#include "MJD.h"

namespace Pulsar {

  //! Plots a single pulse profile
  class ProfilePlot : public FluxPlot, public HasPen
  {

  public:

    //! Default constructor
    ProfilePlot ();

    //! Get the text interface to the configuration attributes
    TextInterface::Parser* get_interface ();

    // Text interface to the ProfilePlot class
    class Interface : public TextInterface::To<ProfilePlot> {
    public:
      Interface (ProfilePlot* = 0);
    };

    //! Load the profiles
    void get_profiles (const Archive* data);

    //! Draw using FluxPlot::draw, then maybe draw_transitions
    void draw (const Archive*);

    //! Set if cal transitions will be drawn
    void set_plot_cal_transitions (bool flag) { plot_cal_transitions = flag; }
    bool get_plot_cal_transitions () const { return plot_cal_transitions; }

    //! Set the threshold used to reject outliers when computing cal levels
    void set_outlier_threshold (float f) { outlier_threshold = f; }
    float get_outlier_threshold () const { return outlier_threshold; }

    //! Set the time to be plotted
    void set_plot_time (const MJD& epoch) { plot_time = epoch; }
    MJD get_plot_time () const { return plot_time; }

    //! Draw the profile transitions in the current viewport and window
    void draw_transitions (const Profile* profile);

    //! Mark the plot_time with a vertical dashed line
    void draw_time ();
    
  protected:

    bool plot_cal_transitions;
    double outlier_threshold;

    MJD plot_time;
    MJD reference_epoch;
    double folding_period{0};
  };

}

#endif
