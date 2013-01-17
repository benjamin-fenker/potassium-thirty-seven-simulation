// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37ScintillatorHit_h
#define K37ScintillatorHit_h 1

#include <vector>
#include <map>

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class G4AttDef;

class K37ScintillatorHit : public G4VHit {
 public:
  K37ScintillatorHit();
  ~K37ScintillatorHit();
  K37ScintillatorHit(const K37ScintillatorHit &right);
  const K37ScintillatorHit& operator=(const K37ScintillatorHit &right);
  G4int operator==(const K37ScintillatorHit &right) const;

  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

  const std::map<G4String, G4AttDef>* GetAttDefs() const;
  std::vector<G4AttValue>* CreateAttValues() const;
  void Print();

 private:
  G4double edep;
  G4ThreeVector pos1;
  G4ThreeVector pos2;
  G4double good;
  G4double time;
  G4bool primary;
  static std::map<G4String, G4AttDef> fAttDefs;

 public:
  inline void SetEdep(G4double de) {
    edep = de;
  }
  inline G4double GetEdep() {
    return edep;
  }

  inline void SetPos1(G4ThreeVector xyz) {
    pos1 = xyz;
  }
  inline void SetPos2(G4ThreeVector xyz) {
    pos2 = xyz;
  }

  inline G4ThreeVector GetPos1() {
    return pos1;
  }
  inline G4ThreeVector GetPos2() {
    return pos2;
  }

  inline void SetBound(G4double go) {
    good = go;
  }
  inline G4double GetBound() {
    return good;
  }

  inline void SetPrimary(G4bool prim) {
    primary = prim;
  }
  inline G4bool GetPrimary() {
    return primary;
  }

  inline void SetTime(G4double tim) {
    time = tim;
  }
  inline G4double GetTime() {
    return time;
  }
};

typedef G4THitsCollection<K37ScintillatorHit> K37ScintillatorHitsCollection;

extern G4Allocator<K37ScintillatorHit> K37ScintillatorHitAllocator;

inline void* K37ScintillatorHit::operator new(size_t) {
  void *aHit;
  aHit = static_cast<void *>(K37ScintillatorHitAllocator.MallocSingle());
  return aHit;
}

inline void K37ScintillatorHit::operator delete(void *aHit) {
  K37ScintillatorHitAllocator.
      FreeSingle(static_cast<K37ScintillatorHit*>(aHit));
}

#endif
