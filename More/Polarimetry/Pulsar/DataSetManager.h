//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2022 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

// psrchive/More/Polarimetry/Pulsar/DataSetManager.h

#ifndef __Pulsar_DataSetManager_H
#define __Pulsar_DataSetManager_H

#include "Pulsar/Database.h"

namespace Pulsar
{
  //! List of input data from a single pulsar and its calibrator observations
  class DataSet : public Reference::Able
  {
  public:

    DataSet ();

    //! Return true if data can be added to this data set
    bool matches (const Archive* data);

    //! Add data to this data set
    void add (const Archive* data);

    //! Get the name of the pulsar in this data set
    const std::string& get_name () { return name; }
    
  protected:

    std::string name;
    double total_integration_length;
    
    std::vector< Reference::To<const Archive> > data;
  };
  
  //! Manages sets of data from multiple sources and a calibrator database
  /*! Each DataSet contains a list of input data from a single pulsar.
    This class manages input data from multiple pulsars using multiple
    DataSet instances that share a common calibrator database. */

  class DataSetManager : public Reference::Able
  {

  public:

    //! Default constructor
    DataSetManager ();

    //! Add to the array of system calibrators
    void manage (DataSet*);

    //! Set the calibrator database
    void set_database (Pulsar::Database* db) { database = db; }
    
    //! Return the reference epoch of the calibration experiment
    MJD get_epoch () const;

    //! Add archive to the appropriate data set
    /*! Add a new dataset if needed */
    void add (const Archive* data);

    //! Get the epoch of the first observation
    MJD get_start_epoch () const;

    //! Get the epoch of the last observation
    MJD get_end_epoch () const;

  protected:

    //! The system calibrators
    std::vector< Reference::To<DataSet> > calibrator;

    //! The database from which PolnCal and FluxCal
    Reference::To<Pulsar::Database> database;
  };

}

#endif

