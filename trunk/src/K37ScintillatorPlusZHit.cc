
#include "K37ScintillatorPlusZHit.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4UnitsTable.hh"
#include "G4AttValue.hh"
#include "G4AttDef.hh"
#include "G4AttCheck.hh"

G4Allocator<K37ScintillatorPlusZHit> K37ScintillatorPlusZHitAllocator;

K37ScintillatorPlusZHit::K37ScintillatorPlusZHit()
{;}

K37ScintillatorPlusZHit::~K37ScintillatorPlusZHit()
{;}

K37ScintillatorPlusZHit::K37ScintillatorPlusZHit(const K37ScintillatorPlusZHit &right)
  : G4VHit()
{
	edep = right.edep;
	pos1 = right.pos1;
	pos2 = right.pos2;
	good = right.good;
	time = right.time;
	primary = right.primary;
}

const K37ScintillatorPlusZHit& K37ScintillatorPlusZHit::operator=(const K37ScintillatorPlusZHit &right)
{
	edep = right.edep;
	pos1 = right.pos1;
	pos2 = right.pos2;
	good = right.good;
	time = right.time;
	primary = right.primary;
	return *this;
}

G4int K37ScintillatorPlusZHit::operator==(const K37ScintillatorPlusZHit &right) const
{
	return (this==&right) ? 1 : 0;
}

std::map<G4String,G4AttDef> K37ScintillatorPlusZHit::fAttDefs;

/*
void K37ScintillatorPlusZHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(pos1);
    circle.SetScreenSize(0.04);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}
*/
 
const std::map<G4String,G4AttDef>* K37ScintillatorPlusZHit::GetAttDefs() const
{
  // G4AttDefs have to have long life.  Use static member...
  if (fAttDefs.empty()) {
    fAttDefs["HitType"] = G4AttDef("HitType","Type of hit","Physics","","G4String");
	fAttDefs["EDep"] = G4AttDef("EDep","Energy deposited","Physics","G4BestUnit","G4double");
  }
  return &fAttDefs;
}

std::vector<G4AttValue>* K37ScintillatorPlusZHit::CreateAttValues() const
{
  // Create expendable G4AttsValues for picking...
  std::vector<G4AttValue>* attValues = new std::vector<G4AttValue>;
  attValues->push_back(G4AttValue("HitType","K37ScintillatorPlusZHit",""));
  attValues->push_back (G4AttValue("EDep",G4BestUnit(edep,"Energy"),""));
  //G4cout << "Checking...\n" << G4AttCheck(attValues, GetAttDefs());
  return attValues;
}

void K37ScintillatorPlusZHit::Print()
{;}


