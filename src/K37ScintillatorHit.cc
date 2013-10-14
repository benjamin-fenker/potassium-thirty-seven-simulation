// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37ScintillatorHit.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4UnitsTable.hh"
#include "G4AttValue.hh"
#include "G4AttDef.hh"
#include "G4AttCheck.hh"

G4Allocator<K37ScintillatorHit> K37ScintillatorHitAllocator;

K37ScintillatorHit::K37ScintillatorHit()
:pos1(G4ThreeVector()),
pos2(G4ThreeVector()),
edep(0),
good(0),
time(0),
particle_pdg(0),
primary(false),
gamma(0)
{;}

K37ScintillatorHit::~K37ScintillatorHit() {
}

K37ScintillatorHit::K37ScintillatorHit(const K37ScintillatorHit &right)
    : G4VHit() {
    pos1 = right.pos1;
    pos2 = right.pos2;
    edep = right.edep;
    good = right.good;
    time = right.time;
    particle_pdg = right.particle_pdg;
    primary = right.primary;
    gamma = right.gamma;
}

const K37ScintillatorHit& K37ScintillatorHit::operator=(
                                          const K37ScintillatorHit &right) {
    pos1 = right.pos1;
    pos2 = right.pos2;
    edep = right.edep;
    good = right.good;
    time = right.time;
    particle_pdg = right.particle_pdg;
    primary = right.primary;
    gamma = right.gamma;
    return *this;
}

G4int K37ScintillatorHit::operator==(const K37ScintillatorHit &right) const {
    return (this == &right) ? 1 : 0;
}

std::map<G4String, G4AttDef> K37ScintillatorHit::fAttDefs;

/*
void K37ScintillatorHit::Draw()
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

const std::map<G4String, G4AttDef>* K37ScintillatorHit::GetAttDefs() const {
    // G4AttDefs have to have long life.  Use static member...
    if (fAttDefs.empty()) {
        fAttDefs["HitType"] = G4AttDef("HitType", "Type of hit", "Physics", "",
                                       "G4String");
        fAttDefs["EDep"] = G4AttDef("EDep", "Energy deposited", "Physics",
                                    "G4BestUnit", "G4double");
    }
    return &fAttDefs;
}

std::vector<G4AttValue>* K37ScintillatorHit::CreateAttValues() const {
    // Create expendable G4AttsValues for picking...
    std::vector<G4AttValue>* attValues = new std::vector<G4AttValue>;
    attValues -> push_back(G4AttValue("HitType", "K37ScintillatorHit", ""));
    attValues -> push_back(G4AttValue("EDep", G4BestUnit(edep, "Energy"), ""));
    // G4cout << "Checking...\n" << G4AttCheck(attValues, GetAttDefs());
    return attValues;
}

void K37ScintillatorHit::Print() {
}


