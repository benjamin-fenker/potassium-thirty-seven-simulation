// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37EventGenerator.hh"

class JTW_Event: public K37EventGenerator {
 public:
  /*int area (){ return (width * height / 2); };*/
  void MakeEvent();
  void MakeEvent(G4double polarization, G4double alignment);
  void ResetGeneratedCounters();
  G4int GetNumMins();
  G4int GetNumPlus();
 private:
  G4int numMins, numPlus;
};
