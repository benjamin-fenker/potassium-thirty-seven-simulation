// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37StripDetectorHit_h
#define K37StripDetectorHit_h 1

#include <map>
#include <vector>

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

using std::map;

class G4AttDef;

class K37StripDetectorHit : public G4VHit {
 public:
  K37StripDetectorHit();
  ~K37StripDetectorHit();
  K37StripDetectorHit(const K37StripDetectorHit &right);
  const K37StripDetectorHit& operator=(const K37StripDetectorHit &right);
  G4int operator==(const K37StripDetectorHit &right) const;

  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

  // void Draw();
  const std::map<G4String, G4AttDef>* GetAttDefs() const;
  std::vector<G4AttValue>* CreateAttValues() const;
  void Print();

 private:
  // Total energy deposited during this hit to all the strips
  G4ThreeVector pos1;           // Position of pre-step point
  G4ThreeVector pos2;           // Position of post-step point
  G4double edep;
  G4double time;                // Global Time
  G4double good;                //
  G4int particle_pdg;
  G4bool primary;
  G4bool SoftwareVeto;
  static std::map<G4String, G4AttDef> fAttDefs;

  // My new stuff starts here
  // Key value represents the strip fired, while value is the energy deposited
  // in that strip during this hit (step).
  map<G4int, G4double> xStripsEDep, yStripsEDep;


 public:
  map<G4int, G4double> getXStripsEdep();
  map<G4int, G4double> getYStripsEdep();

  inline void SetPos1(G4ThreeVector xyz) {pos1 = xyz;}
  inline G4ThreeVector GetPos1() {return pos1;}
  // ----------------------------------------------
  inline void SetPos2(G4ThreeVector xyz) {pos2 = xyz;}
  inline G4ThreeVector GetPos2() {return pos2;}
  // ----------------------------------------------
  inline void SetEdep(G4double de) {edep = de;}
  inline G4double GetEdep() {return edep;}
  // ----------------------------------------------
  inline void SetBound(G4double go) {good = go;}
  inline G4double GetBound() {return good;}
  // ----------------------------------------------
  inline void SetTime(G4double tim) {time = tim;}
  inline G4double GetTime() {return time;}
  // ----------------------------------------------
  inline void SetParticlePDG(G4int pdg_) {particle_pdg = pdg_;}
  inline G4int GetParticlePDG() {return particle_pdg;}
  // ----------------------------------------------
  inline void SetPrimary(G4bool prim) {primary = prim;}
  inline G4bool GetPrimary() {return primary;}
  // ----------------------------------------------
  inline void SetSoftwareVeto(G4bool sof) { SoftwareVeto = sof; }
  inline G4double GetSoftwareVeto() { return SoftwareVeto; }

  void addXEdepPair(G4int strip, G4double edep);
  void addYEdepPair(G4int strip, G4double edep);
  void setXMap(map<G4int, G4double> xMap);
  void setYMap(map<G4int, G4double> yMap);
};

typedef G4THitsCollection<K37StripDetectorHit> K37StripDetectorHitsCollection;

extern G4Allocator<K37StripDetectorHit> K37StripDetectorHitAllocator;

inline void* K37StripDetectorHit::operator new(size_t) {
  void *aHit;
  aHit = static_cast<void *>(K37StripDetectorHitAllocator.MallocSingle());
  return aHit;
}

inline void K37StripDetectorHit::operator delete(void *aHit) {
  K37StripDetectorHitAllocator.
      FreeSingle(static_cast<K37StripDetectorHit*>(aHit));
}

#endif


