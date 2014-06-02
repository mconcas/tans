#include <TFile.h>
#include "TString.h"
#include "TChain.h"
#include "TProof.h"
#include "TSystem.h"

void ReconSteer( const Bool_t  fProof=kTRUE,
                 const TString fDataFile="events.root", 
                 const TString fOption="force") {
   TString option;
   if(fOption.Contains("force")) option="kfg";
   else option="kg";
   gSystem->CompileMacro("Punto.cxx",option);
   gSystem->CompileMacro("Direzione.cxx",option);
   gSystem->CompileMacro("Vertice.cxx",option);
   gSystem->CompileMacro("Hit.cxx",option);

   ///////////////////////////////////////////////////////////////////
   // Open data file.
   TFile *fFile=TFile::Open(fDataFile.Data());
   if(fFile->IsZombie()) {
      Printf("[ERROR] There was a problem accessing %s file. Please\
         check if it exists. ", fDataFile.Data());
      return;
   }
   TChain *fEventChain=new TChain("Events Tree");
   fEventChain->Add(fDataFile.Data());
   if(fProof) {
      TString fWorkerString;
      TProof::Open("workers=4");
      fEventChain->SetProof();

      // Fixed, it seems it doesn't need for this "loads".
      // gProof->SetParameter("PROOF_UseTreeCache",(Int_t) 0); 
      // gProof->Load("Punto.cxx+");
      // gProof->Load("Vertice.cxx+");
      // gProof->Load("Direzione.cxx+");
      // gProof->Load("Hit.cxx+");

      fEventChain->Process("EventSelector.C+");
   }
   else {
      TTree *SerTree = (TTree*)fFile->Get("Events Tree");
      SerTree->Process("EventSelector.C+");
   }

   fFile->Close();
}
