// Authors: Spencer Behling and Benjamin Fenker 2013

#include "K37HistogramManager.hh"
#include "G4UnitsTable.hh"

#ifdef G4ANALYSIS_USE
#include "AIDA/AIDA.h"
#endif

//-----------------------------------------

K37HistogramManager::K37HistogramManager()
  :af(0), tree(0), factoryOn(false) {
#ifdef G4ANALYSIS_USE
  // Creating the analysis factory
  af = AIDA_createAnalysisFactory();
  if (!af) {
    G4cout << " K37HistogramManager::K37HistogramManager() :"
           << " problem creating the AIDA analysis factory." << G4endl;
  }
#endif

  fileName[0] = "detectors";
  fileType    = "root";
  fileOption  = "";
  ntupl0 = 0;
  ntupl1 = 0;
  ntupl2 = 0;
  ntupl3 = 0;
}

//-----------------------------------------

K37HistogramManager::~K37HistogramManager() {
#ifdef G4ANALYSIS_USE
  delete af;
#endif
}

//-----------------------------------------

void K37HistogramManager::book() {
#ifdef G4ANALYSIS_USE
  if (!af) return;

  // Creating a tree mapped to an hbook file.
  fileName[1] = fileName[0] + "." + fileType;
  G4bool readOnly  = false;
  G4bool createNew = true;
  AIDA::ITreeFactory* tf  = af->createTreeFactory();
  tree = tf->create(fileName[1], fileType, readOnly, createNew, fileOption);
  delete tf;
  if (!tree) {
    G4cout << "K37HistogramManager::book() :"
           << " problem creating the AIDA tree with "
           << " storeName = " << fileName[1]
           << " storeType = " << fileType
           << " readOnly = "  << readOnly
           << " createNew = " << createNew
           << " options = "   << fileOption
           << G4endl;
    return;
  }

  // Creating a histogram & ntuplr factory
  AIDA::IHistogramFactory* hf = af->createHistogramFactory(*tree);
  AIDA::ITupleFactory* ntf = af->createTupleFactory(*tree);

  ntupl0 = ntf->create("StripDetectorPlusZ", "StripDetectorPlusZ", "double e");
  ntupl1 = ntf->create("ScintillatorPlusZ", "ScintillatorPlusZ", "double e");
  ntupl2 = ntf->create("StripDetectorMinusZ", "StripDetectorMinusZ",
                       "double e");
  ntupl3 = ntf->create("ScintillatorMinusZ", "ScintillatorMinusZ", "double e");
  factoryOn = true;

  delete hf;
  delete ntf;

  if (factoryOn)
    G4cout << "\n----> Histogram Tree is opened in " << fileName[1] << G4endl;
#endif
}

//-----------------------------------------

void K37HistogramManager::save() {
#ifdef G4ANALYSIS_USE
  if (factoryOn) {
    tree->commit();       // Writing the histograms to the file
    tree->close();        // and closing the tree (and the file)
    G4cout << "\n----> Histogram Tree is saved in " << fileName[1] << G4endl;

    delete tree;
    tree = 0;
    factoryOn = false;
  }
#endif
}

//-----------------------------------------

void K37HistogramManager::FillNtuple(G4int nt, G4int column, G4double value) {
  if (nt >= MaxNtupl) {
    G4cout << "---> warning from K37HistogramManager::FillNtuple() : Ntuple "
           << nt << " does not exist " << column << value << G4endl;
    return;
  }
#ifdef G4ANALYSIS_USE
  if (nt == 0) ntupl0->fill(column, value);
  if (nt == 1) ntupl1->fill(column, value);
  if (nt == 2) ntupl2->fill(column, value);
  if (nt == 3) ntupl3->fill(column, value);
#endif
}

//-----------------------------------------

void K37HistogramManager::AddRowNtuple(G4int nt) {
  if (nt >= MaxNtupl) {
    G4cout << "---> warning from K37HistogramManager::AddRowNtuple() : Ntuple "
           << nt << " do not exist" << G4endl;
    return;
  }
#ifdef G4ANALYSIS_USE
  if (nt == 0) ntupl0->addRow();
  if (nt == 1) ntupl1->addRow();
  if (nt == 2) ntupl2->addRow();
  if (nt == 3) ntupl3->addRow();
#endif
}

//-----------------------------------------
