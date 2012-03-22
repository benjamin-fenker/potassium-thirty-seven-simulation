

#include "K37StripDetectorPlusZHit.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4AttValue.hh"
#include "G4AttDef.hh"
#include "G4AttCheck.hh"

G4Allocator<K37StripDetectorPlusZHit> K37StripDetectorPlusZHitAllocator;

K37StripDetectorPlusZHit::K37StripDetectorPlusZHit()
{;}

K37StripDetectorPlusZHit::~K37StripDetectorPlusZHit()
{;}

K37StripDetectorPlusZHit::K37StripDetectorPlusZHit(const K37StripDetectorPlusZHit &right)
  : G4VHit()
{
	edep = right.edep;
	pos1 = right.pos1;
	pos2 = right.pos2;
	good = right.good;
	time = right.time;
	primary = right.primary;
	SoftwareVeto = right.SoftwareVeto;
}

const K37StripDetectorPlusZHit& K37StripDetectorPlusZHit::operator=(const K37StripDetectorPlusZHit &right)
{
	edep = right.edep;
	pos1 = right.pos1;
	pos2 = right.pos2;
	good = right.good;
	time = right.time;
	SoftwareVeto = right.SoftwareVeto;
	return *this;
}

G4int K37StripDetectorPlusZHit::operator==(const K37StripDetectorPlusZHit &right) const
{
  return (this==&right) ? 1 : 0;
}

std::map<G4String,G4AttDef> K37StripDetectorPlusZHit::fAttDefs;

/*
void K37StripDetectorPlusZHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(pos1);
    circle.SetScreenSize(0.04);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(0.,0.,1.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}
*/

const std::map<G4String,G4AttDef>* K37StripDetectorPlusZHit::GetAttDefs() const
{
  // G4AttDefs have to have long life.  Use static member...
  if (fAttDefs.empty()) {
    fAttDefs["HitType"] = G4AttDef("HitType","Type of hit","Physics","","G4String");
    fAttDefs["EDep"] = G4AttDef("EDep","Energy deposited","Physics","G4BestUnit","G4double");
  }
  return &fAttDefs;
}

std::vector<G4AttValue>* K37StripDetectorPlusZHit::CreateAttValues() const
{
  // Create expendable G4AttsValues for picking...
  std::vector<G4AttValue>* attValues = new std::vector<G4AttValue>;
  attValues->push_back (G4AttValue("HitType","K37StripDetectorPlusZHit",""));
  attValues->push_back (G4AttValue("EDep",G4BestUnit(edep,"Energy"),""));
  //G4cout << "Checking...\n" << G4AttCheck(attValues, GetAttDefs());
  return attValues;
}

void K37StripDetectorPlusZHit::Print()
{;}


