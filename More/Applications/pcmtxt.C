/***************************************************************************
 *
 *   Copyright (C) 2023 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/Application.h"
#include "Pulsar/ConfigurableProjectionExtension.h"
#include "Pulsar/ConfigurableProjection.h"

#include "pairutil.h"

using namespace std;
using namespace Pulsar;

//
//! Prints data extracted/derived from pcm solutions as text
//
class pcmtxt : public Pulsar::Application
{
public:

  //! Default constructor
  pcmtxt ();

  //! Process the given archive
  void process (Pulsar::Archive*);

  //! Print configurable projection model evaluated on a two-dimensional grid
  void print (ConfigurableProjectionExtension* ext);

protected:

  //! Print a two-dimensional grid of configurable projection model
  bool configurable_projection_grid;

  //! the abscissa index on each axis
  std::pair<unsigned,unsigned> configurable_projection_abscissae; 

  //! the index of the model parameter value to be printed at each point of grid
  unsigned configurable_projection_model_index;

  //! the frequency channel for which to extract a grid
  unsigned grid_ichan;

  //! the number of grid points on each axis
  std::pair<unsigned,unsigned> grid_points; 

  //! the minimum and maximum value on each axis
  std::pair<double, double> grid_range[2]; 

  //! Add command line options
  void add_options (CommandLine::Menu&);
};


pcmtxt::pcmtxt ()
  : Application ("pcmtxt", "print data extracted/derived from pcm.fits files")
{
  configurable_projection_grid = false;

  configurable_projection_model_index = 0;
  configurable_projection_abscissae.first = 0;
  configurable_projection_abscissae.second = 1;

  grid_ichan = 0;

  // a 100x100 grid
  grid_points.first = 100;
  grid_points.second = 100;

  // range in hour angle (degrees)
  grid_range[0].first = -20;
  grid_range[0].second = +20;

  // range in declination (degrees)
  grid_range[1].first = -60;
  grid_range[1].second = +30;
}


/*!

  Add application-specific command-line options.

*/

void pcmtxt::add_options (CommandLine::Menu& menu)
{
  CommandLine::Argument* arg;

  // add a blank line and a header to the output of -h
  menu.add ("\n" "Configurable projection grid options:");

  // enable printing the configurable projection
  arg = menu.add (configurable_projection_grid, "cp");
  arg->set_help ("print configurable projection parameter on a grid");

  // set the indeces on each axis
  arg = menu.add (configurable_projection_model_index, 'm', "index");
  arg->set_help ("index of configurable projection model parameter");

  // set the indeces on each axis
  arg = menu.add (configurable_projection_abscissae, 'a', "ix,iy");
  arg->set_help ("configurable projection abscissa index for each grid axis");

  // add a blank line and a header to the output of -h
  menu.add ("\n" "Generic grid options:");

  // set the frequency axis
  arg = menu.add (grid_ichan, 'c', "ichan");
  arg->set_help ("frequency channel index of model data");

  // set the indeces on each axis
  arg = menu.add (grid_points, 'N', "Nx,Ny");
  arg->set_help ("number of points on each grid axis");

  // set the range on x-axis
  arg = menu.add (grid_range[0], 'x', "xmin,xmax");
  arg->set_help ("range on grid x-axis");

  // set the range on y-axis
  arg = menu.add (grid_range[1], 'y', "ymin,ymax");
  arg->set_help ("range on grid y-axis");
}


/*!

  Scale every profile and optionally set the source name

*/

void pcmtxt::process (Pulsar::Archive* archive)
{
  if (configurable_projection_grid)
  {
    auto ext = archive->get<ConfigurableProjectionExtension>();
    if (!ext)
      throw Error (InvalidParam, "pcmtxt::process", "archive does not contain a ConfigurableProjectionExtension");

    print (ext);
  }
}

void pcmtxt::print (ConfigurableProjectionExtension* ext)
{
  auto projection = new ConfigurableProjection (ext);

  auto xform = projection->get_transformation (grid_ichan)->get_transformation();

  if (!xform->has_constraint(configurable_projection_model_index))
    throw Error (InvalidParam, "pcmtxt::print", 
                "model index=%u is not constrained", configurable_projection_model_index);

  auto model = xform->get_model();
}


/*!

  The standard C/C++ main function simply calls Application::main

*/

int main (int argc, char** argv)
{
  pcmtxt program;
  return program.main (argc, argv);
}

