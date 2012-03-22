
#ifndef K37Ar37Minus_h
#define K37Ar37Minus_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4Ions.hh"

// ######################################################################
// ###                        Ar37Minus                               ###
// ######################################################################

class K37Ar37Minus : public G4Ions
//class K37Ar37Minus : G4ParticleDefinition
{
 private:
   static K37Ar37Minus* theInstance;
   K37Ar37Minus(){}
   ~K37Ar37Minus(){}

 public:
   static K37Ar37Minus* Definition();
   static K37Ar37Minus* Ar37MinusDefinition();
   static K37Ar37Minus* Ar37Minus();
};

#endif
