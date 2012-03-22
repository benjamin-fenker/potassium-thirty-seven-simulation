

#include "K37MirrorHit.hh"
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

G4Allocator<K37MirrorHit> K37MirrorHitAllocator;

K37MirrorHit::K37MirrorHit()
{;}

K37MirrorHit::~K37MirrorHit()
{;}

K37MirrorHit::K37MirrorHit(const K37MirrorHit &right)
  : G4VHit()
{
  edep = right.edep;
  pos1 = right.pos1;
  pos2 = right.pos2;
  good = right.good;
  time = right.time;
}

const K37MirrorHit& K37MirrorHit::operator=(const K37MirrorHit &right)
{
  edep = right.edep;
  pos1 = right.pos1;
  pos2 = right.pos2;
  good = right.good;
  time = right.time;
  return *this;
}

G4int K37MirrorHit::operator==(const K37MirrorHit &right) const
{
  return (this==&right) ? 1 : 0;
}

std::map<G4String,G4AttDef> K37MirrorHit::fAttDefs;
/*
void K37MirrorHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(pos1);
    circle.SetScreenSize(0.04);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(0.,1.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}
*/

const std::map<G4String,G4AttDef>* K37MirrorHit::GetAttDefs() const
{
  // G4AttDefs have to have long life.  Use static member...
  if (fAttDefs.empty()) {
    fAttDefs["HitType"] = G4AttDef("HitType","Type of hit","Physics","","G4String");
    fAttDefs["EDep"] = G4AttDef("EDep","Energy deposited","Physics","G4BestUnit","G4double");
  }
  return &fAttDefs;
}

std::vector<G4AttValue>* K37MirrorHit::CreateAttValues() const
{
  // Create expendable G4AttsValues for picking...
  std::vector<G4AttValue>* attValues = new std::vector<G4AttValue>;
  attValues->push_back (G4AttValue("HitType","K37MirrorHit",""));
  attValues->push_back (G4AttValue("EDep",G4BestUnit(edep,"Energy"),""));
  //G4cout << "Checking...\n" << G4AttCheck(attValues, GetAttDefs());
  return attValues;
}

void K37MirrorHit::Print()
{;}


