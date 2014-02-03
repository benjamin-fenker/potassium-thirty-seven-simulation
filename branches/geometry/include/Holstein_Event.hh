// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37EventGenerator.hh"

class Holstein_Event: public K37EventGenerator {
 public:
  /*int area (void) { return (width * height); };*/
  void MakeEvent(G4double polarization, G4double alignment,
                 G4double charge_state);
  void MakeEvent();
};
