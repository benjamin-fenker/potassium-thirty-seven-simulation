

#ifndef K37StripDetectorMinusZSD_h
#define K37StripDetectorMinusZSD_h 1

#include "G4VSensitiveDetector.hh"
#include "K37StripDetectorMinusZHit.hh"
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class K37StripDetectorMinusZSD : public G4VSensitiveDetector
{

  public:
      K37StripDetectorMinusZSD(G4String name);
      ~K37StripDetectorMinusZSD();

      void Initialize(G4HCofThisEvent*HCE);
      G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
      void EndOfEvent(G4HCofThisEvent* HCE);
      void clear();
      void DrawAll();
      void PrintAll();

  private:
      K37StripDetectorMinusZHitsCollection *dedx2Collection;

};




#endif

