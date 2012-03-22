
#ifndef K37ScintillatorPlusZSD_h
#define K37ScintillatorPlusZSD_h 1

#include "G4VSensitiveDetector.hh"
#include "K37ScintillatorPlusZHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class K37ScintillatorPlusZSD : public G4VSensitiveDetector
{

  public:
      K37ScintillatorPlusZSD(G4String name);
      ~K37ScintillatorPlusZSD();

      void Initialize(G4HCofThisEvent*HCE);
      G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
      void EndOfEvent(G4HCofThisEvent*HCE);
      void clear();
      void DrawAll();
      void PrintAll();

  private:
      K37ScintillatorPlusZHitsCollection *fullenergy1Collection;

  public:
};




#endif

