// Authors: Spencer Behling and Benjamin Fenker 2013

#include "Holstein_Event.hh"
#include "globals.hh"

void Holstein_Event::MakeEvent(G4double polarization, G4double alignment) {
  G4cout << "Holstein is working \t P = " << polarization << "\t T = "
         << alignment << G4endl;
}

void Holstein_Event::MakeEvent() {
  MakeEvent(1.0, 1.0);
}
