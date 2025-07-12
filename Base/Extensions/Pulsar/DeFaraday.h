//-*-C++-*-
/***************************************************************************
 *
 *   Copyright (C) 2006-2025 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/

// psrchive/Base/Extensions/Pulsar/DeFaraday.h

#ifndef __Pulsar_DeFaraday_h
#define __Pulsar_DeFaraday_h

#include "Pulsar/ColdPlasmaHistory.h"

namespace Pulsar {
  
  //! Stores parameters used to correct Faraday rotation in each Integration
  class DeFaraday : public ColdPlasmaHistory
  {
  public:
    
    //! Default constructor
    DeFaraday ();

    //! Copy constructor
    DeFaraday (const DeFaraday& extension);

    //! Assignment operator
    const DeFaraday& operator= (const DeFaraday& extension);
    
    //! Clone method
    DeFaraday* clone () const { return new DeFaraday( *this ); }
  };
  
}

#endif

