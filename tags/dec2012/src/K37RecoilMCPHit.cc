// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37RecoilMCPHit.hh"

G4Allocator<K37RecoilMCPHit> K37RecoilMCPHitAllocator;

K37RecoilMCPHit::K37RecoilMCPHit()
:energy(0),
time(0),
x_pos(0),
z_pos(0),
particle_pdg(0),
particle_charge(0)
{;}

K37RecoilMCPHit::~K37RecoilMCPHit()
{;}

K37RecoilMCPHit::K37RecoilMCPHit(const K37RecoilMCPHit &right)
    : G4VHit() {
    energy = right.energy;
    time   = right.time;
    x_pos  = right.x_pos;
    z_pos  = right.z_pos;
    particle_pdg = right.particle_pdg;
    particle_charge = right.particle_charge;
}

const K37RecoilMCPHit& K37RecoilMCPHit::operator=(
                                          const K37RecoilMCPHit &right) {
    energy = right.energy;
    time   = right.time;
    x_pos  = right.x_pos;
    z_pos  = right.z_pos;
    particle_pdg = right.particle_pdg;
    particle_charge = right.particle_charge;
    return *this;
}

G4int K37RecoilMCPHit::operator==(const K37RecoilMCPHit &right) const {
    return (this == &right) ? 1 : 0;
}


