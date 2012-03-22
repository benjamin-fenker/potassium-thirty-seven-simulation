
#ifndef K37StripDetectorMinusZHit_h
#define K37StripDetectorMinusZHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"


class G4AttDef;

class K37StripDetectorMinusZHit : public G4VHit
{
  public:

      K37StripDetectorMinusZHit();
      ~K37StripDetectorMinusZHit();
      K37StripDetectorMinusZHit(const K37StripDetectorMinusZHit &right);
      const K37StripDetectorMinusZHit& operator=(const K37StripDetectorMinusZHit &right);
      G4int operator==(const K37StripDetectorMinusZHit &right) const;

      inline void *operator new(size_t);
      inline void operator delete(void *aHit);

     // void Draw();
      const std::map<G4String,G4AttDef>* GetAttDefs() const;
      std::vector<G4AttValue>* CreateAttValues() const;
      void Print();

  private:
      G4double edep;
      G4ThreeVector pos1;
      G4ThreeVector pos2;
      G4double time;
      G4double good;
      G4bool primary;
      static std::map<G4String,G4AttDef> fAttDefs;

public:

	inline void SetEdep(G4double de)
	{ edep = de; }
	inline G4double GetEdep()
	{ return edep; }
	
	inline void SetPos1(G4ThreeVector xyz)
	{ pos1 = xyz; }
	inline void SetPos2(G4ThreeVector xyz)
	{ pos2 = xyz; }
	  
	inline G4ThreeVector GetPos1()
	{ return pos1; }
	inline G4ThreeVector GetPos2()
	{ return pos2; }
	  
	inline void SetPrimary(G4bool prim)
	{ primary = prim; }
	inline G4bool GetPrimary()
	{ return primary; }
	
	inline void SetBound(G4double go)
	{ good = go; }
	inline G4double GetBound()
	{ return good; }
	  
	inline void SetTime(G4double tim)
	{ time = tim; }
	inline G4double GetTime()
	{ return time; }
	  
      
};

typedef G4THitsCollection<K37StripDetectorMinusZHit> K37StripDetectorMinusZHitsCollection;

extern G4Allocator<K37StripDetectorMinusZHit> K37StripDetectorMinusZHitAllocator;

inline void* K37StripDetectorMinusZHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) K37StripDetectorMinusZHitAllocator.MallocSingle();
  return aHit;
}

inline void K37StripDetectorMinusZHit::operator delete(void *aHit)
{
  K37StripDetectorMinusZHitAllocator.FreeSingle((K37StripDetectorMinusZHit*) aHit);
}

#endif


