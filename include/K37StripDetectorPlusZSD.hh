

#ifndef K37StripDetectorPlusZSD_h
#define K37StripDetectorPlusZSD_h 1

#include "G4VSensitiveDetector.hh"
#include "K37StripDetectorPlusZHit.hh"
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class K37StripDetectorPlusZSD : public G4VSensitiveDetector
{

  public:
      K37StripDetectorPlusZSD(G4String name);
      ~K37StripDetectorPlusZSD();

      void Initialize(G4HCofThisEvent*HCE);
      G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
      void EndOfEvent(G4HCofThisEvent* HCE);
      void clear();
      void DrawAll();
      void PrintAll();

  private:
	K37StripDetectorPlusZHitsCollection *dedx1Collection;
	G4double X;
	G4double Y;
	G4double Z;
	
};




#endif

