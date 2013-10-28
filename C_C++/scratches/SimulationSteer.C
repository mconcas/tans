#include "TFile.h"
#include "TString.h"
#include "TChain.h"
#include "TProof.h"
#include "TSystem.h"
#define NUM 9



void steer(const TString avecProof="noProof",
            const TString proofString="workers=4") {
   TFile *f = TFile::Open("vertices.root");
   if (!f) {
  	Printf("no can do.");
  	return;
   }
   else Printf("File \"vertices.root\" is opened.");

   TChain *mychain = new TChain("Vertices");
   mychain->Add("vertices.root");
   Printf("TFile added to mychain.");
   if (avecProof=="noProof") {
      TTree *mytree = (TTree *)f->Get("Vertices");

      mytree->Process("SimulationSelector.C+");
   }
   else {                        
      TProof::Open(proofString.Data());
      mychain->SetProof();
      gProof->Load("Punto.cxx+");
      gProof->Load("Vertice.cxx+");
      gProof->Load("Direzione.cxx+");
      gProof->Load("Hit.cxx+");

   }
   mychain->Process("SimulationSelector.C+");
}
