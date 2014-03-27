#define EventSelector_cxx
// The class definition in EventSelector.h has been generated
// automatically by the ROOT utility TTree::MakeSelector().
// This class is derived from the ROOT class TSelector.
// For more information on the TSelector framework see
// $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.
// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only
//                    on the slave servers.
//    Process():      called for each event, in this function you
//                    decide what to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree,
//                    when on PROOF called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("EventSelector.C")
// Root > T->Process("EventSelector.C","some options")
// Root > T->Process("EventSelector.C+")

#include "EventSelector.h"
#include <Vertice.h>
#include <Hit.h>
#include <Direzione.h>
#include <TH2.h>
#include <TMath.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TSystem.h>

Bool_t EventSelector::Notify() {
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new
   // TTree is started when using PROOF. It is normally not necessary
   // to make changes to the generated code, but the routine can be
   // extended by the user if needed.
   // The return value is currently not used.

   return kTRUE;
}

void EventSelector::Init(TTree *tree) {
   // The Init() function is called when the selector needs
   // to initialize a new tree or chain.
   // Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetBranchAddress("Vertices",&fVertex);
   fChain->SetBranchAddress("Firstlayer",&fHitsFirstLayer);
   fChain->SetBranchAddress("Secondlayer",&fHitsSecondLayer);

}

void EventSelector::Begin(TTree *) {
   Printf("+++++++++ Beginning processing on «%s» +++++++++",
      gSystem->HostName());
}

void EventSelector::SlaveBegin(TTree *) {
   // fHitsFirstLayer = new TClonesArray("Hit");
   fHistPhi = new TH1F("HistPhi","Scarti sull'azimuth",360,
      -0.01*TMath::Pi(),0.01*TMath::Pi());
   // Add the output histogram to the margiable object list.
   fOutput->Add( fHistPhi );
}
//////////////////////////////////////////////////////////////////////
// Core of the analysis.

Bool_t EventSelector::Process(Long64_t entry) {

   Printf("Processing entry n°: %lld", entry+1);
   ++fNumberOfEvents;
   fChain->GetEvent( entry );

   Int_t fEntriesLOne = fHitsFirstLayer->GetEntries();
   Int_t fEntriesLTwo = fHitsSecondLayer->GetEntries();

   const Double_t fAngle=0.174532925;
   for ( Int_t v=0; v<fEntriesLTwo; v++ ) {
      fAnaHitScnd=(Hit*)fHitsSecondLayer->At(v);
      fAnaHitScnd->Hit::GausSmearing(70,0.12,0.003);
      for (Int_t j=0; j<fEntriesLOne; j++) {
         fAnaHitFrst=(Hit*)fHitsFirstLayer->At(j);
         fAnaHitFrst->Hit::GausSmearing(40,0.12,0.003);
         if( TMath::Abs(fAnaHitScnd->GetPuntoPhi()-
            fAnaHitFrst->GetPuntoPhi())<=fAngle/2 )
            fHistPhi->Fill(fAnaHitScnd->GetPuntoPhi()-
            fAnaHitFrst->GetPuntoPhi());
      }
   }


   return kTRUE;
}

void EventSelector::SlaveTerminate() {
   // The SlaveTerminate() function is called after all entries or
   // objects have been processed.
   // When running with PROOF SlaveTerminate() is called
   // on each slave server.
   delete fHitsFirstLayer;
   delete fHitsSecondLayer;
   printf("\nTotal Number of Events: %d\n", fNumberOfEvents);


}

void EventSelector::Terminate() {
   // The Terminate() function is the last function to be called
   // during a query. It always runs on the client, it can be used
   // to present the results graphically or save the results to file.

   fHistPhi=dynamic_cast<TH1F*>(
      fOutput->FindObject("HistPhi") );
   if (fHistPhi) fHistPhi->Draw();
}
