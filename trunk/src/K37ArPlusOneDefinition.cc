// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37ArPlusOneDefinition.hh"
#include "G4ParticleTable.hh"

// ######################################################################
// ###                        Ar37PlusOne                             ###
// ######################################################################

K37Ar37PlusOne* K37Ar37PlusOne::theInstance = 0;

K37Ar37PlusOne* K37Ar37PlusOne::Definition() {
    if (theInstance !=0) return theInstance;
    const G4String name = "Ar37PlusOne";
    // search in particle table
    G4ParticleTable* pTable = G4ParticleTable::GetParticleTable();
    G4Ions* anInstance = reinterpret_cast<G4Ions*>(pTable->FindParticle(name));
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

        anInstance = new G4Ions(
                 name,                34.43382*GeV,       0.0*MeV,  1.0*eplus,
                    3,                          +1,             0,
                    0,                           0,             0,
            "nucleus",                           0,           +37,  1000180370,
                 true,                        -1.0,          NULL,
                false,                    "static",   -1000180370,
                  0.0);
    }
    theInstance = reinterpret_cast<K37Ar37PlusOne*>(anInstance);
    return theInstance;
}

K37Ar37PlusOne*  K37Ar37PlusOne::Ar37PlusOneDefinition() {
    return Definition();
}

K37Ar37PlusOne*  K37Ar37PlusOne::Ar37PlusOne() {
    return Definition();
}


