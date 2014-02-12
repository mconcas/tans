#include <TFile.h>
#include "TString.h"
#include "TChain.h"
#include "TProof.h"
#include "TSystem.h"

void Steer(const TString fDataFile="events.root", 
           Bool_t fProof=kTRUE) {
   TFile *fFile=TFile::Open(fDataFile.Data());
   if(fFile->IsZombie()) {
      Printf("[ERROR] There was a problem accessing %s file. Please\
         check if it exists. ", fDataFile.Data());
      return;
   }
   TChain *fEventChain=new TChain("EventChain");
   fEventChain->Add(fDataFile.Data());
   if(fProof) {
      TString fWorkerString;
      TProof::Open("workers=4");
      fEventChain->SetProof();
      gProof->Load("Punto.cxx+");
      gProof->Load("Vertice.cxx+");
      gProof->Load("Direzione.cxx+");
      gProof->Load("Hit.cxx+");

   }
   fEventChain->Process("EventSelector.C+");
}
