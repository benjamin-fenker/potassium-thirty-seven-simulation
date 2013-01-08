#include "K37StripDetectorHit.hh"
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

using std::pair;

G4Allocator<K37StripDetectorHit> K37StripDetectorHitAllocator;

K37StripDetectorHit::K37StripDetectorHit()
{;}

K37StripDetectorHit::~K37StripDetectorHit()
{;}

K37StripDetectorHit::K37StripDetectorHit(const K37StripDetectorHit &right)
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

const K37StripDetectorHit& K37StripDetectorHit::operator=(const K37StripDetectorHit &right)
{
	edep = right.edep;
	pos1 = right.pos1;
	pos2 = right.pos2;
	good = right.good;
	time = right.time;
	SoftwareVeto = right.SoftwareVeto;
	return *this;
}

G4int K37StripDetectorHit::operator==(const K37StripDetectorHit &right) const
{
  return (this==&right) ? 1 : 0;
}

std::map<G4String,G4AttDef> K37StripDetectorHit::fAttDefs;

/*
void K37StripDetectorHit::Draw()
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

const std::map<G4String,G4AttDef>* K37StripDetectorHit::GetAttDefs() const
{
  // G4AttDefs have to have long life.  Use static member...
  if (fAttDefs.empty()) {
    fAttDefs["HitType"] = G4AttDef("HitType","Type of hit","Physics","","G4String");
    fAttDefs["EDep"] = G4AttDef("EDep","Energy deposited","Physics","G4BestUnit","G4double");
  }
  return &fAttDefs;
}

std::vector<G4AttValue>* K37StripDetectorHit::CreateAttValues() const
{
  // Create expendable G4AttsValues for picking...
  std::vector<G4AttValue>* attValues = new std::vector<G4AttValue>;
  attValues->push_back (G4AttValue("HitType","K37StripDetectorHit",""));
  attValues->push_back (G4AttValue("EDep",G4BestUnit(edep,"Energy"),""));
  //G4cout << "Checking...\n" << G4AttCheck(attValues, GetAttDefs());
  return attValues;
}

void K37StripDetectorHit::Print()
{;}

void K37StripDetectorHit::addXEdepPair(G4int strip, G4double edep) {
  if (edep < 0.0) {
    G4cerr << "ERROR.  K37StripDetectorHit::addXEdepPair.  edep < 0.0";
    G4cerr << G4endl;
    exit(1);
  }
  xStripsEDep.insert(pair<G4int, G4double>(strip, edep));
}

void K37StripDetectorHit::addYEdepPair(G4int strip, G4double edep) {
  if (edep < 0.0) {
    G4cerr << "ERROR.  K37StripDetectorHit::addXEdepPair.  edep < 0.0";
    G4cerr << G4endl;
    exit(1);
  }
  yStripsEDep.insert(pair<G4int, G4double>(strip, edep));
}

void K37StripDetectorHit::setXMap(map<G4int, G4double> xMap) {
  // First check if any edep is < 0.  That would be bad.
  // This object will loop over all map elements
  map<G4int, G4double>::iterator it;
  for (it = xMap.begin(); it != xMap.end(); it++) {
    if ((*it).second < 0.0) {
      G4cerr << "K37StripDetectorHit::setXMap Error!!!" << G4endl;
      G4cerr << "edep less than zero for strip " << (*it).first << G4endl;
      exit(1);
    }
  }
  xStripsEDep = xMap;
}

void K37StripDetectorHit::setYMap(map<G4int, G4double> yMap) {
  // First check if any edep is < 0.  That would be bad.
  // This object will loop over all map elements
  map<G4int, G4double>::iterator it;
  for (it = yMap.begin(); it != yMap.end(); it++) {
    if ((*it).second < 0.0) {
      G4cerr << "K37StripDetectorHit::setYMap Error!!!" << G4endl;
      G4cerr << "edep less than zero for strip " << (*it).first << G4endl;
      exit(1);
    }
  }
  yStripsEDep = yMap;
}

map<G4int, G4double> K37StripDetectorHit::getXStripsEdep() {
  return xStripsEDep;
}

map<G4int, G4double> K37StripDetectorHit::getYStripsEdep() {
  return yStripsEDep;
}

