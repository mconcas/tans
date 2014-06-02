#if !defined (__CINT__) || defined (__MAKECINT__)
#include <TFile.h>
#include "TString.h"
#include "TChain.h"
#include "TProof.h"
#include "TSystem.h"
#endif

void ReconSteer(const TString Selector="ReconSelector.cxx+", 
   const Bool_t Proof=kFALSE,
   TString Target="data/AllDisabled_ZeroNoise.root",
   const TString Treename="Events Tree",
   const TString Option="force")
{
   // (Re)Compile classes, macros, etc.
   TString option;
   if(Option.Contains("force")) option="kfg";
   else option="kg";
   gSystem->CompileMacro("Punto.cxx",option);
   gSystem->CompileMacro("Direzione.cxx",option);
   gSystem->CompileMacro("Vertice.cxx",option);
   gSystem->CompileMacro("Hit.cxx",option);

   Target=Target;
   TChain *EventChain=new TChain(Treename.Data());
   TString FileName;
   for(Int_t i=0;i<6;++i) {
      TString Prefix="data/Noise_X_Multscatt_disabled_events_1M/";
      FileName.Form("%sNoise_%d_Multscatt_disabled_events_1M.root",
         Prefix.Data(),i*6);
      EventChain->Add(FileName.Data());
      Printf("\x1B[34mAdded %s to the TChain.\x1B[0m",FileName.Data());
   }
   // EventChain->Add(Target);
   // Printf("\x1B[34mAdded %s to the TChain.\x1B[0m",Target.Data());

   if(Proof) {
      Printf("\x1B[31m +++ Beginning Reconstruction +++\x1B[0m");
      Printf("\x1B[31m +++ Analyzed tree name:   %s\x1B[0m",Treename.Data());
      Printf("\x1B[31m +++ Proof master name:    %s\x1B[0m",
         gSystem->HostName());
      Printf("\x1B[31m +++ Selector chosen:      %s\x1B[0m",Selector.Data());
 
      TString fWorkerString;
      TProof::Open("workers=2");
      // gProof->SetParameter("PROOF_FeedbackPeriod", (Long_t)14400000);
      EventChain->SetProof();

      gProof->Load("Punto.cxx+");
      gProof->Load("Vertice.cxx+");
      gProof->Load("Direzione.cxx+");
      gProof->Load("Hit.cxx+");

      EventChain->Process(Selector.Data());
   } else {
      EventChain->Process(Selector.Data());
   }
}
