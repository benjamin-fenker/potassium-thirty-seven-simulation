
#ifndef K37StripDetectorHit_h
#define K37StripDetectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"


class G4AttDef;

class K37StripDetectorHit : public G4VHit
{
  public:

      K37StripDetectorHit();
      ~K37StripDetectorHit();
      K37StripDetectorHit(const K37StripDetectorHit &right);
      const K37StripDetectorHit& operator=(const K37StripDetectorHit &right);
      G4int operator==(const K37StripDetectorHit &right) const;

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
	G4bool SoftwareVeto;
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
	
	inline void SetSoftwareVeto(G4bool sof)
	{ SoftwareVeto = sof; }
	inline G4double GetSoftwareVeto()
	{ return SoftwareVeto; }
	  
      
};

typedef G4THitsCollection<K37StripDetectorHit> K37StripDetectorHitsCollection;

extern G4Allocator<K37StripDetectorHit> K37StripDetectorHitAllocator;

inline void* K37StripDetectorHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) K37StripDetectorHitAllocator.MallocSingle();
  return aHit;
}

inline void K37StripDetectorHit::operator delete(void *aHit)
{
  K37StripDetectorHitAllocator.FreeSingle((K37StripDetectorHit*) aHit);
}

#endif


