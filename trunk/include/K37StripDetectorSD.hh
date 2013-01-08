

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
  void SetupParameters(G4ThreeVector pos, G4int num, G4double width);
  // Fills placementInWorld, numStrips, stripWidth
private:
  map<G4int, G4double> MapOfEDep(G4int preStrip, G4int postStrip,
                                 G4double prePos, G4double postPos,
                                 G4double totalEDep);
  K37StripDetectorHitsCollection *dedx1Collection;
  G4double X;             // X - position of pre-step point
  G4double Y;             // Y - position of pre-step point
  G4double Z;             // X^2 + Y^2 - Why???
  // Apparently Z greater or less than 156.25 is a cutoff for the software veto?
  // Why 156.25???

  // Position of the center of the SD in the world coordinates
  G4ThreeVector placementInWorld;
  G4int numStrips;              // Forty strips in x; forty in y
  G4double stripWidth;          // 1 mm wide for each strip in x and y
};




#endif

