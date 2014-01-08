// Authors: Spencer Behling and Benjamin Fenker 2013

//   This class manages the verbose outputs in G4SteppingManager.
//   It inherits from G4SteppingVerbose.
//   It shows how to extract informations during the tracking of a particle.
//
// ----------------------------------------------
// ----------------------------------------------

class K37SteppingVerbose;

#ifndef K37SteppingVerbose_h
#define K37SteppingVerbose_h 1

#include "G4SteppingVerbose.hh"

// ----------------------------------------------

class K37SteppingVerbose : public G4SteppingVerbose {
 public:
  K37SteppingVerbose();
  ~K37SteppingVerbose();

  void StepInfo();
  void TrackingStarted();
};

// ----------------------------------------------

#endif
