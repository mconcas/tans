#include "TFile.h"
#include "TString.h"
#include "TChain.h"
#include "TProof.h"
#include "TSystem.h"
//#define NUM 9



void steer(const TString avecProof="noProof", const TString proofString="workers=4", const Int_t NUM =9) {
  TFile *f = TFile::Open("htree/htree_000.root");
  if (!f) {
    Printf("no can do");
    return;
  }

  TChain *mychain = new TChain("T");
  TString buf;
  for (Int_t i=0; i<=NUM; i++) {
    buf.Form("htree/htree_%03d.root", i);
    Printf("Adding to chain: %s", buf.Data());
    mychain->Add( buf.Data() );
  }
  if (avecProof=="noProof") {
    //gSystem->Load("Punto.cxx+");
    //TTree *mytree = (TTree *)f->Get("T");
    mychain->Process("NewSelector.C+");
    }
  else {
    TProof::Open(proofString.Data());

    mychain->SetProof();
    gProof->Load("Punto.cxx+");
  }
  mychain->Process("NewSelector.C+");

}
