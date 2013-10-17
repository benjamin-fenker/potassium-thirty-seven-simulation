// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37ElectronMCPHit.hh"

G4Allocator<K37ElectronMCPHit> K37ElectronMCPHitHitAllocator;

K37ElectronMCPHit::K37ElectronMCPHit()
:energy(0),
time(0),
particle_pdg(0)
{;}

K37ElectronMCPHit::~K37ElectronMCPHit()
{;}

K37ElectronMCPHit::K37ElectronMCPHit(const K37ElectronMCPHit &right)
  : G4VHit() {
    energy = right.energy;
    time = right.time;
    particle_pdg = right.particle_pdg;
}

const K37ElectronMCPHit& K37ElectronMCPHit::operator=(
                                          const K37ElectronMCPHit &right) {
    energy = right.energy;
    time = right.time;
    particle_pdg = right.particle_pdg;
    return *this;
}

G4int K37ElectronMCPHit::operator==(const K37ElectronMCPHit &right) const {
    return (this == &right) ? 1 : 0;
}



