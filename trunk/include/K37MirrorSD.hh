

#ifndef K37MirrorSD_h
#define K37MirrorSD_h 1

#include "G4VSensitiveDetector.hh"
#include "K37MirrorHit.hh"
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class K37MirrorSD : public G4VSensitiveDetector
{

  public:
      K37MirrorSD(G4String name);
      ~K37MirrorSD();

      void Initialize(G4HCofThisEvent*HCE);
      G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
      void EndOfEvent(G4HCofThisEvent* HCE);
      void clear();
      void DrawAll();
      void PrintAll();

  private:
      K37MirrorHitsCollection *mirrorCollection;

};




#endif

