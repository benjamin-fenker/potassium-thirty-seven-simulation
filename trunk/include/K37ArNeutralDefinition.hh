// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37Ar37Neutral_h
#define K37Ar37Neutral_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4Ions.hh"

// ######################################################################
// ###                        Ar37Neutral                             ###
// ######################################################################

class K37Ar37Neutral : public G4Ions {
 private:
  static K37Ar37Neutral* theInstance;
  K37Ar37Neutral() {}
  ~K37Ar37Neutral() {}

 public:
  static K37Ar37Neutral* Definition();
  static K37Ar37Neutral* Ar37NeutralDefinition();
  static K37Ar37Neutral* Ar37Neutral();
};

#endif
