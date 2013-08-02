// Authors: Spencer Behling and Benjamin Fenker 2013

// ROOT headers
#include <TTree.h>
#include <TDirectory.h>
#include <TROOT.h>
#include <TBranchRef.h>

// C++ headers
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <map>
// C++ wrapped C
#include <cmath>
#include <cassert>

// Project Specific
#include "K37_Data.hh"
//#include "K37RunData.hh"
#include "Aggregator.hh"

using std::cerr;
using std::endl;
using std::cout;
using std::left;
using std::setw;
using std::sqrt;
using std::abs;
using std::map;

namespace AGG {
class Aggregator::ImplementAGG {
 public:
  ImplementAGG();
  ~ImplementAGG();
  void InsertDataStream(K37_ABC::K37_Data *dataPointer_);
  void InsertChannel(const std::string &uniqueName_,
                     K37_ABC::K37_Data *uniquePointer_);

  void InitilzeIO(const std::string &fileName_);
  void OpenOutputChannel(const std::string &method_,
                         const std::string &fileName_);

  void Print(std::ofstream &fileToWrite_);
  void PrintHeader(std::ofstream &fileToWrite_);
  void Print(std::ostream &fileToWrite_);
  void PrintHeader(std::ostream &fileToWrite_);
  void PrintROOT();
  void InitilzeOutput();
  void PrintDataToOutput();
  void ClearData();
  void EndRun();
  map<std::string, K37_ABC::K37_Data * > dataMap;
  std::map<std::string, K37_ABC::K37_Data * >::iterator it_data;
  std::ofstream outputFile;
  TFile *rootFile;
  TFile* GetRootFile() {return rootFile;}
  bool textFileOpen;
  bool writeToScreen;
  bool writeToROOT;
  TTree *theNTuple;
  TDirectory *theDirectory;
  void WriteRunData();
};

Aggregator::ImplementAGG::ImplementAGG()
    : rootFile(0),
      textFileOpen(false),
      writeToScreen(false),
      writeToROOT(false),
      theNTuple(0) {
  dataMap.clear();
}

Aggregator::ImplementAGG::~ImplementAGG() {
  dataMap.clear();

  if (textFileOpen == true) {
    outputFile.close();
  }

  // if (writeToROOT == true)
  // {
  //   rootFile->cd();
  //   theNTuple->Write(0, TObject::kOverwrite);
  //   rootFile->Close();
  // }

  // Do not attempt to delete any of the root objects that were created
  // using new. For example the TFile or the NTuple. ROOT does not live
  // by the normal C++ rules of an object that allocates memory should
  // free it. ROOT just likes to inflict pain on the world by using
  // arbitrary design pattens that make no sense. Deal with it.
}

void Aggregator::ImplementAGG::OpenOutputChannel(const std::string &method_,
                                                 const std::string &fileName_) {
  if (method_ == "text") {
    outputFile.open(fileName_.c_str(), std::ofstream::trunc);
    textFileOpen = outputFile.is_open();
  } else if (method_ == "screen") {
    writeToScreen = true;
  } else if (method_ == "root") {
    std::string variableNames;

    for (it_data = dataMap.begin(); it_data != dataMap.end(); ++it_data) {
      if (it_data == dataMap.begin()) {
        variableNames = (*it_data).first;
      } else {
        variableNames += (":"+ (*it_data).first);
      }
    }
    // you have to use a pointer to a TFile and then use new to create
    // an instance. Using any other design pattern screws up
    // roots "supposedly easy" automatic association of object with
    // files.

    if (rootFile != 0) {
      rootFile -> cd();
      rootFile -> Close();
      delete rootFile;
      rootFile = 0;
      theNTuple = 0;
    }

    if (theNTuple != 0) {
      delete theNTuple;
      theNTuple = 0;
    }

    rootFile = new TFile(fileName_.c_str(), "RECREATE", "", 0);
    theNTuple = new TTree("ntuple", "TheTuple");
    std::string columnName;
    std::string typeBit;
    std::string typeName;

    for (it_data = dataMap.begin(); it_data != dataMap.end(); ++it_data) {
      // (*it_data).second -> Print();
      columnName = (*it_data).first;
      if ((*it_data).second) {
        typeBit = ((*it_data).second)->GetType();
        typeName = columnName + typeBit;
        if (typeBit == "/D") {
          theNTuple->Branch(columnName.c_str(),
                            ((*it_data).second)->GetReferenceD(),
                            typeName.c_str());
        }
        if (typeBit == "/l") {
          theNTuple->Branch(columnName.c_str(),
                            ((*it_data).second)->GetReferenceI(),
                            typeName.c_str());
        }
        if (typeBit == "/v") {
          theNTuple->Branch(columnName.c_str(),
                            "std::vector<double>",
                            ((*it_data).second)->GetReferenceV());
        }
      }
    }
    writeToROOT = true;
  } else {
    cerr << "Unknown output method selected check configuration file." << endl;
    assert(0);
  }

  if ((writeToScreen == false) && (textFileOpen == false)
      && (writeToROOT == false)) {
    cerr << "Problem opening selected output stream" << endl;
    assert(0);
  }
}

void Aggregator::ImplementAGG::InitilzeOutput() {
  if (textFileOpen == true) {
    PrintHeader(outputFile);
  }

  if (writeToScreen == true) {
    PrintHeader(std::cout);
  }
}

void Aggregator::ImplementAGG::PrintDataToOutput() {
  if (textFileOpen == true) {
    Print(outputFile);
  }

  if (writeToScreen == true) {
    Print(std::cout);
  }

  if (writeToROOT == true) {
    PrintROOT();
  }

  ClearData();
}

void Aggregator::ImplementAGG::InitilzeIO(const std::string &fileName_) {
  std::fstream fileToRead(fileName_.c_str(), std::fstream::in);
  std::string outputFileName;
  std::string outputMethod;

  while (fileToRead >> outputMethod >> outputFileName) {
    OpenOutputChannel(outputMethod, outputFileName);
  }
  fileToRead.close();
}

void Aggregator::ImplementAGG::InsertChannel(const std::string &uniqueName_,
                                         K37_ABC::K37_Data *uniquePointer_) {
  dataMap.insert(std::make_pair(uniqueName_, uniquePointer_));
}

void Aggregator::ImplementAGG::EndRun() {
  WriteRunData();

  if (writeToROOT == true) {
    rootFile -> cd();
    theNTuple -> Write(0, TObject::kOverwrite);
    rootFile -> Close();
    delete rootFile;
    rootFile = 0;
    theNTuple = 0;
  }
  dataMap.clear();
  writeToScreen = false;
  writeToROOT = false;
}

void Aggregator::ImplementAGG::InsertDataStream
(K37_ABC::K37_Data *dataPointer_) {
  InsertChannel(dataPointer_->GetName(), dataPointer_);
}

void Aggregator::ImplementAGG::PrintHeader(std::ofstream &fileToWrite_) {
  for (it_data = dataMap.begin(); it_data != dataMap.end(); ++it_data) {
    fileToWrite_ << (*it_data).first << " ";
  }
  fileToWrite_ << endl;
}

void Aggregator::ImplementAGG::PrintHeader(std::ostream &fileToWrite_) {
  for (it_data = dataMap.begin(); it_data != dataMap.end(); ++it_data) {
    fileToWrite_ << (*it_data).first << " ";
  }
  fileToWrite_ << endl;
}

void Aggregator::ImplementAGG::ClearData() {
  for (it_data = dataMap.begin(); it_data != dataMap.end(); ++it_data) {
    ((*it_data).second)->Clear();
  }
}

void Aggregator::ImplementAGG::Print(std::ofstream &fileToWrite_) {
  for (it_data = dataMap.begin(); it_data != dataMap.end(); ++it_data) {
    fileToWrite_ << ((*it_data).second)->Print();
  }
  fileToWrite_ << endl;
}

void Aggregator::ImplementAGG::Print(std::ostream &fileToWrite_) {
  for (it_data = dataMap.begin(); it_data != dataMap.end(); ++it_data) {
    std::cout << " I got to here" << std::endl;
    fileToWrite_ << ((*it_data).second)->Print();
  }
  fileToWrite_ << endl;
}

void Aggregator::ImplementAGG::PrintROOT() {
  theNTuple->Fill();
}

void Aggregator::ImplementAGG::WriteRunData() {
  // K37RunData *data = new K37RunData();
  //  cout << "Polarization: " << data -> GetPolarization() << endl;
  //  cout << "Writing my new thingy!!!" << endl;
  // rootFile -> cd();
  // data -> Write();
}

Aggregator::Aggregator()
    :mAGG(new ImplementAGG()) {
}

Aggregator::~Aggregator() {
  delete mAGG;
  mAGG = NULL;
}

void Aggregator::RegisterData(K37_ABC::K37_Data *dataPointer_) {
  mAGG->InsertDataStream(dataPointer_);
}

void Aggregator::BeginRun() {
  mAGG->InitilzeOutput();
}

void Aggregator::EndRun() {
  mAGG -> EndRun();
}

void Aggregator::EndEvent() {
  mAGG->PrintDataToOutput();
}

void Aggregator::RegisterIOMethod(const std::string &fileName_) {
  mAGG->InitilzeIO(fileName_);
}
  
TFile* Aggregator::GetRootFile() {
  return mAGG -> GetRootFile();
}

}

