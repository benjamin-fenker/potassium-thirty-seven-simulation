#include <cstdlib>
#include <iostream>

#include <TArrayD.h>
#include <TBranch.h>
#include <TBranchElement.h>
#include <TBranchRef.h>
#include <TLeafElement.h>
#include <TLeaf.h>
#include <TLeafD.h>
#include <TLeafL.h>
#include <TRint.h>
#include <TFile.h>
#include <TTree.h>

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cout << "Must have two command line arguments." << endl;
    cout << "run ./GrabParameters paramsFromHere.root toHere.root" << endl;
    return(1);
  }
  cout << argv[1] << endl;
  TRint *theApp = new TRint("Rint", &argc, argv);
  cout << "the file: " << theApp -> Argv(1) << endl;
  TFile *in = new TFile(theApp -> Argv(1), "READ");
  if (in -> IsZombie()) {
    cout << "Failed because file " << theApp -> Argv(1) << "doesn't exist or isn't"
         << " openable" << endl;
    exit(1);
  }
  TFile *out = new TFile(theApp -> Argv(2), "UPDATE");
  if (out -> IsZombie()) {
    cout << "Failed because file " << theApp -> Argv(2) << "doesn't exist or isn't"
         << "openable" << endl;
    exit(1);
  }

  TArrayD *tad;
  in ->GetObject("RunParameters", tad);
  out -> cd();
  out -> WriteObject(tad, "RunParameters");
  out -> Close();
  in -> Close();
  delete in;
  delete out;
  return 0;
}
