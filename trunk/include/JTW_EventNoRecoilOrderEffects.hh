// Authors: Spencer Behling
//This is a hack to add in a two percent branch

#include "K37EventGeneratorNoRecoilOrderEffects.hh"

class JTW_EventNoRecoilOrderEffects: public K37EventGeneratorNoRecoilOrderEffects {
 public:
  /*int area (){ return (width * height / 2); };*/
  void MakeEvent();
  void MakeEvent(G4double polarization, G4double alignment,
                 G4double recoil_charge);
  void ResetGeneratedCounters();
  G4int GetNumMins();
  G4int GetNumPlus();
 private:
  G4int numMins, numPlus;
};
