// Authors: Spencer Behling and Benjamin Fenker 2013

#ifndef K37MirrorHit_h
#define K37MirrorHit_h 1

#include <map>
#include <vector>

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class G4AttDef;

class K37MirrorHit : public G4VHit {
 public:
  K37MirrorHit();
  ~K37MirrorHit();
  K37MirrorHit(const K37MirrorHit &right);
  const K37MirrorHit& operator=(const K37MirrorHit &right);
  G4int operator==(const K37MirrorHit &right) const;

  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

  // void Draw();
  const std::map<G4String, G4AttDef>* GetAttDefs() const;
  std::vector<G4AttValue>* CreateAttValues() const;
  void Print();

 private:
  G4double edep;
  G4ThreeVector pos1;
  G4ThreeVector pos2;
  G4double time;
  G4double good;

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

  inline void SetTime(G4double tim) {
    time = tim;
  }
  inline G4double GetTime() {
    return time;
  }
};

typedef G4THitsCollection<K37MirrorHit> K37MirrorHitsCollection;

extern G4Allocator<K37MirrorHit> K37MirrorHitAllocator;

inline void* K37MirrorHit::operator new(size_t) {
  void *aHit;
  aHit = static_cast<void *>(K37MirrorHitAllocator.MallocSingle());
  return aHit;
}

inline void K37MirrorHit::operator delete(void *aHit) {
  K37MirrorHitAllocator.FreeSingle(static_cast<K37MirrorHit*>(aHit));
}

#endif


