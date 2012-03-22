
#ifndef K37ScintillatorMinusZSD_h
#define K37ScintillatorMinusZSD_h 1

#include "G4VSensitiveDetector.hh"
#include "K37ScintillatorMinusZHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class K37ScintillatorMinusZSD : public G4VSensitiveDetector
{

  public:
      K37ScintillatorMinusZSD(G4String name);
      ~K37ScintillatorMinusZSD();

      void Initialize(G4HCofThisEvent*HCE);
      G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
      void EndOfEvent(G4HCofThisEvent*HCE);
      void clear();
      void DrawAll();
      void PrintAll();

  private:
      K37ScintillatorMinusZHitsCollection *fullenergy2Collection;

  public:
};




#endif

