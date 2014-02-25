#define EventSelector_cxx
// The class definition in EventSelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("EventSelector.C")
// Root > T->Process("EventSelector.C","some options")
// Root > T->Process("EventSelector.C+")
//

#include "EventSelector.h"
#include <Vertice.h>
#include <Hit.h>
#include <Direzione.h>
#include <TH2.h>
#include <TH1I.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TSystem.h>

Bool_t EventSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void EventSelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
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
void EventSelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   //TString option = GetOption();
   Printf("<><><><><> Begin Function on Host: %s <><><><><>", 
      gSystem->HostName());
}

void EventSelector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).
   // TString option = GetOption();

   fHitsFirstLayer = new TClonesArray("Hit");
   fPuppetHistIO = new TH1I("HistIO","Z Histogram",100,-30,30);
   fOutput->Add( fPuppetHistIO );
}

Bool_t EventSelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either EventSelector::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.
   Printf("Processing Entry number %lld\n", entry);
   ++fNumberOfEvents;

   fChain->GetEvent( entry );
   Int_t num = fHitsFirstLayer->GetEntries();

   Printf("Ev=%lld Mult=%d(%d) X=%lf Y=%lf Z=%lf",
      entry,
      fVertex->GetVerticeMult(), num, fVertex->GetPuntoX(), 
      fVertex->GetPuntoY(), fVertex->GetPuntoZ());
   Hit *tst;
   for (Int_t j=0; j<num; j++){
      tst=(Hit*)fHitsFirstLayer->At(j);
      Printf("  #%d: X=%lf Y=%lf Z=%lf",
        j, tst->GetPuntoX(), tst->GetPuntoY(), tst->GetPuntoZ());
      fPuppetHistIO->Fill( tst->GetPuntoX() );
      if(fPuppetHistIO) Printf("-------------------------");
   }


   return kTRUE;
}

void EventSelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   delete fHitsFirstLayer;
   printf("\nTotal Number of Events: %d\n", fNumberOfEvents);


}

void EventSelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   fPuppetHistIO=dynamic_cast<TH1I*>( 
      fOutput->FindObject("HistIO") );
   if (fPuppetHistIO) 
      fPuppetHistIO->Draw();

}
