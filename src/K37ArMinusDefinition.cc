// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37ArMinusDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4PhaseSpaceDecayChannel.hh"
#include "G4DecayTable.hh"

// ######################################################################
// ###                        Ar37Minus                               ###
// ######################################################################

K37Ar37Minus* K37Ar37Minus::theInstance = 0;

K37Ar37Minus* K37Ar37Minus::Definition() {
    if (theInstance !=0) return theInstance;
    const G4String name = "Ar37Minus";
    // search in particle table
    G4ParticleTable* pTable = G4ParticleTable::GetParticleTable();
    G4Ions* anInstance = reinterpret_cast<G4Ions*>(pTable->FindParticle(name));
    // G4ParticleDefinition* anInstance =
    //   reinterpret_cast< G4ParticleDefinition*>(pTable->FindParticle(name));
    if (anInstance ==0) {
        // create particle
        //
        //    Arguments for constructor are as follows
        //               name             mass          width         charge
        //             2*spin           parity  C-conjugation
        //          2*Isospin       2*Isospin3       G-parity
        //               type    lepton number  baryon number   PDG encoding
        //             stable         lifetime    decay table
        //             shortlived      subType    anti_encoding
        //             excitation
      anInstance = new G4Ions(name, 34.4348410923*GeV, 2.5315e-27*GeV,
                              -1.0*eplus, 0, +1, 0, 0, 0, 0, "nucleus", 0, +37,
                              1000180370, false, 260.0*ns, NULL, false,
                              "static", -1000180370, 0.0);

        // create Decay Table
        G4DecayTable* table = new G4DecayTable();
        // create decay channel: Ar37Minus -> Ar + e-
        G4PhaseSpaceDecayChannel* mode =
          new G4PhaseSpaceDecayChannel("Ar37Minus", 1, 2, "Ar37Neutral", "e-");

        table->Insert(mode);
        // delete mode;

        anInstance->SetDecayTable(table);
    }
    theInstance = reinterpret_cast<K37Ar37Minus*>(anInstance);
    return theInstance;
}

K37Ar37Minus*  K37Ar37Minus::Ar37MinusDefinition() {
    return Definition();
}

K37Ar37Minus*  K37Ar37Minus::Ar37Minus() {
    return Definition();
}


