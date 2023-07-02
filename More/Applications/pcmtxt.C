/***************************************************************************
 *
 *   Copyright (C) 2023 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

#include "Pulsar/Application.h"
#include "Pulsar/ConfigurableProjectionExtension.h"
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

  //! the parameter index on each axis
  std::pair<unsigned,unsigned> configurable_projection_index; 

  //! the number of grid points on each axis
  std::pair<unsigned,unsigned> configurable_projection_points; 

  //! the minimum and maximum value on each axis
  std::pair<double, double> configurable_projection_range[2]; 

  //! Add command line options
  void add_options (CommandLine::Menu&);
};


pcmtxt::pcmtxt ()
  : Application ("pcmtxt", "print data extracted/derived from pcm.fits files")
{
  configurable_projection_grid = false;

  configurable_projection_index.first = 0;
  configurable_projection_index.second = 1;

  // a 100x100 grid
  configurable_projection_points.first = 100;
  configurable_projection_points.second = 100;

  // range in hour angle (degrees)
  configurable_projection_range[0].first = -20;
  configurable_projection_range[0].second = +20;

  // range in declination (degrees)
  configurable_projection_range[1].first = -60;
  configurable_projection_range[1].second = +30;
}


/*!

  Add application-specific command-line options.

*/

void pcmtxt::add_options (CommandLine::Menu& menu)
{
  CommandLine::Argument* arg;

  // add a blank line and a header to the output of -h
  menu.add ("\n" "Configurable projection grid options:");

  // set the indeces on each axis
  arg = menu.add (configurable_projection_grid, "cp");
  arg->set_help ("print the configurable projection evaluated on a grid");

  // set the indeces on each axis
  arg = menu.add (configurable_projection_index, 'i', "ix,iy");
  arg->set_help ("configurable projection model index on each grid axis");

  // set the indeces on each axis
  arg = menu.add (configurable_projection_points, 'N', "Nx,Ny");
  arg->set_help ("number of points on each grid axis");

  // set the range on x-axis
  arg = menu.add (configurable_projection_points, 'x', "xmin,xmax");
  arg->set_help ("range on grid x-axis");

  // set the range on y-axis
  arg = menu.add (configurable_projection_points, 'y', "ymin,ymax");
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

}



/*!

  The standard C/C++ main function simply calls Application::main

*/

int main (int argc, char** argv)
{
  pcmtxt program;
  return program.main (argc, argv);
}

