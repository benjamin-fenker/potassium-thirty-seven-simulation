

#ifndef K37StripDetectorSD_h
#define K37StripDetectorSD_h 1

#include "G4VSensitiveDetector.hh"
#include "K37StripDetectorHit.hh"
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class K37StripDetectorSD : public G4VSensitiveDetector
{

  public:
      K37StripDetectorSD(G4String name);
      ~K37StripDetectorSD();

      void Initialize(G4HCofThisEvent*HCE);
      G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
      void EndOfEvent(G4HCofThisEvent* HCE);
      void clear();
      void DrawAll();
      void PrintAll();

  private:
	K37StripDetectorHitsCollection *dedx1Collection;
	G4double X;
	G4double Y;
	G4double Z;
	
};




#endif

