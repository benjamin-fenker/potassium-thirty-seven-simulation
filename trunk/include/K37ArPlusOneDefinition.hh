// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37Ar37PlusOne_h
#define K37Ar37PlusOne_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4Ions.hh"

// ######################################################################
// ###                        Ar37PlusOne                             ###
// ######################################################################

class K37Ar37PlusOne : public G4Ions {
 private:
  static K37Ar37PlusOne* theInstance;
  K37Ar37PlusOne() {}
  ~K37Ar37PlusOne() {}

 public:
  static K37Ar37PlusOne* Definition();
  static K37Ar37PlusOne* Ar37PlusOneDefinition();
  static K37Ar37PlusOne* Ar37PlusOne();
};

#endif
