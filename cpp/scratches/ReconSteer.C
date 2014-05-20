#if !defined (__CINT__) || defined (__MAKECINT__)
#include <TFile.h>
#include "TString.h"
#include "TChain.h"
#include "TProof.h"
#include "TSystem.h"
#endif

void ReconSteer(
   const TString Datafile="Simulation_fixed_vertex_with_scattering.root",
   const TString Selector="ReconSelector.cxx+",
   const Bool_t Proof=kFALSE,
   const TString Treename="Events Tree",const TString Option="force")
{

   // (Re)Compile classes, macros, etc.
   TString option;
   if(Option.Contains("force")) option="kfg";
   else option="kg";
   gSystem->CompileMacro("Punto.cxx",option);
   gSystem->CompileMacro("Direzione.cxx",option);
   gSystem->CompileMacro("Vertice.cxx",option);
   gSystem->CompileMacro("Hit.cxx",option);

   // Open data file.
   TFile *fFile=TFile::Open(Datafile.Data());
   if(fFile->IsZombie()) {
      Printf("[ERROR] There was a problem accessing %s file. Please\
         check if it exists. ", Datafile.Data());
      return;
   }
   TChain *fEventChain=new TChain(Treename.Data());
   fEventChain->Add(Datafile.Data());
   if(Proof) {
      Printf(" \x1B[32m+++ Beginning Reconstruction +++");
      Printf(" +++ Reading from file:    %s",Datafile.Data());
      Printf(" +++ Analyzed tree name:   %s",Treename.Data());
      Printf(" +++ Proof master name:    %s",gSystem->HostName());
      Printf(" +++ Selector chosen:      %s\x1B[0",Selector.Data());

      TString fWorkerString;
      TProof::Open("workers=4");
      fEventChain->SetProof();

      gProof->Load("Punto.cxx+");
      gProof->Load("Vertice.cxx+");
      gProof->Load("Direzione.cxx+");
      gProof->Load("Hit.cxx+");

      fEventChain->Process(Selector.Data());
   } else {
      TTree *SerTree = (TTree*)fFile->Get(Treename.Data());
      SerTree->Process(Selector.Data());
   }

   fFile->Close();
}
