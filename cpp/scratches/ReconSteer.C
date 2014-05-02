#if !defined (__CINT__) || defined (__MAKECINT__)
#include <TFile.h>
#include "TString.h"
#include "TChain.h"
#include "TProof.h"
#include "TSystem.h"
#endif

void ReconSteer(const Bool_t  fProof=kTRUE, 
                const TString fDataFile="events.root",
                const TString fTreeName="Events Tree",
                const TString fSelectorName="DeltaPhiSelector.cxx+",
                const TString fOption="force") {

   // (Re)Compile classes, macros, etc.
   TString option;
   if(fOption.Contains("force")) option="kfg";
   else option="kg";
   gSystem->CompileMacro("Punto.cxx",option);
   gSystem->CompileMacro("Direzione.cxx",option);
   gSystem->CompileMacro("Vertice.cxx",option);
   gSystem->CompileMacro("Hit.cxx",option);

   // Open data file.
   TFile *fFile=TFile::Open(fDataFile.Data());
   if(fFile->IsZombie()) {
      Printf("[ERROR] There was a problem accessing %s file. Please\
         check if it exists. ", fDataFile.Data());
      return;
   }
   TChain *fEventChain=new TChain(fTreeName.Data());
   fEventChain->Add(fDataFile.Data());
   if(fProof) {
      Printf(" +++ Beginning reconstruction +++");
      Printf(" +++ Reading from file:    %s", fDataFile.Data());
      Printf(" +++ Analyzed tree name:   %s", fTreeName.Data());
      Printf(" +++ Proof master name:    %s", gSystem->HostName());
      TString fWorkerString;
      TProof::Open("workers=4");
      fEventChain->SetProof();

      gProof->Load("Punto.cxx+");
      gProof->Load("Vertice.cxx+");
      gProof->Load("Direzione.cxx+");
      gProof->Load("Hit.cxx+");

      fEventChain->Process(fSelectorName.Data());
   }
   else {
      TTree *SerTree = (TTree*)fFile->Get(fTreeName.Data());
      SerTree->Process(fSelectorName.Data());
   }

   fFile->Close();
}
