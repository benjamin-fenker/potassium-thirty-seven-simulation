
#ifndef K37ScintillatorSD_h
#define K37ScintillatorSD_h 1

#include "G4VSensitiveDetector.hh"
#include "K37ScintillatorHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class K37ScintillatorSD : public G4VSensitiveDetector
{

  public:
      K37ScintillatorSD(G4String name);
      ~K37ScintillatorSD();

      void Initialize(G4HCofThisEvent*HCE);
      G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
      void EndOfEvent(G4HCofThisEvent*HCE);
      void clear();
      void DrawAll();
      void PrintAll();

  private:
      K37ScintillatorHitsCollection *fullenergy1Collection;

  public:
};




#endif

